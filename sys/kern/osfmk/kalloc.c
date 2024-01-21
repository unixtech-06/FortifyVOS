/*
 * Copyright (c) 2000-2020 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 *
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */
/*
 * @OSF_COPYRIGHT@
 */
/*
 * Mach Operating System
 * Copyright (c) 1991,1990,1989,1988,1987 Carnegie Mellon University
 * All Rights Reserved.
 *
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 *
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 *
 * Carnegie Mellon requests users of this software to return to
 *
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 *
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */
/*
 */
/*
 *	File:	kern/kalloc.c
 *	Author:	Avadis Tevanian, Jr.
 *	Date:	1985
 *
 *	General kernel memory allocator.  This allocator is designed
 *	to be used by the kernel to manage dynamic memory fast.
 */

#include "assert.h"
#include "kalloc.h"
#include "kern_types.h"
#include "zalloc.h"


#define MAX_SIZE_ZDLUT     ((KALLOC_DLUT_SIZE - 1) * KALLOC_MINALIGN)
#define INDEX_ZDLUT(size)       (((size) + KALLOC_MINALIGN - 1) / KALLOC_MINALIGN)

SECURITY_READ_ONLY_LATE(vm_size_t) kalloc_max_prerounded;

static TUNABLE(bool, kheap_temp_debug, "kheap_temp_debug", false);


zone_t
kalloc_heap_zone_for_size(kalloc_heap_t kheap, vm_size_t size)
{
	struct kheap_zones *khz = kheap->kh_zones;

	if (size < MAX_SIZE_ZDLUT) {
		uint32_t zindex = khz->dlut[INDEX_ZDLUT(size)];
		return khz->k_zone[zindex];
	}

	if (size < kalloc_max_prerounded) {
		uint32_t zindex = khz->k_zindex_start;
		while (khz->cfg[zindex].kzc_size < size) {
			zindex++;
		}
		assert(zindex < khz->max_k_zone);
		return khz->k_zone[zindex];
	}

	return ZONE_NULL;
}

struct kalloc_result kalloc_ext(
    kalloc_heap_t         kheap,
    vm_size_t             req_size,
    zalloc_flags_t        flags,
    vm_allocation_site_t  *site)
{
	vm_size_t size;
	void *addr;
	zone_t z;

	/*
	 * Check for debugging flags in DEBUG or DEVELOPMENT builds.
	 * If kheap_temp_debug is enabled and the heap is KHEAP_TEMP,
	 * adjust the requested size to account for the temporary header.
	 */
#if DEBUG || DEVELOPMENT
	if (__improbable(kheap_temp_debug)) {
		if (kheap == KHEAP_TEMP) {
			req_size += sizeof(struct kheap_temp_header);
		}
	}
#endif /* DEBUG || DEVELOPMENT */

	// Since KASAN (Kernel Address Sanitizer) is removed, the requested size
	// is directly assigned to the size variable without any modification.
	size = req_size;

	// Determine the appropriate memory zone for the requested size.
	z = kalloc_heap_zone_for_size(kheap, size);

	// If no valid zone is found, allocate from the larger pool of memory.
	if (__improbable(z == ZONE_NULL)) {
		return kalloc_large(kheap, req_size, size, flags, site);
	}

	/*
	 * In KALLOC_DEBUG mode, ensure the requested size does not exceed
	 * the maximum element size of the selected zone. Trigger a panic if it does.
	 */
#ifdef KALLOC_DEBUG
	if (size > zone_elem_size(z)) {
		panic("%s: z %p (%s%s) but requested size %lu", __func__, z,
		    kalloc_heap_names[kheap->kh_zones->heap_id], z->z_name,
		    (unsigned long)size);
	}
#endif
	// Assert that the requested size is within the limits of the zone's element size.
	assert(size <= zone_elem_size(z));

	/*
	 * If VM tags are enabled and the zone supports tagging, allocate
	 * a tag for this memory allocation and attach it to the flags.
	 */
#if VM_MAX_TAG_ZONES
	if (z->tags) {
		vm_tag_t tag = zalloc_flags_get_tag(flags);
		if (tag == VM_KERN_MEMORY_NONE && site) {
			tag = vm_tag_alloc(site);
		}
		if (tag != VM_KERN_MEMORY_NONE) {
			tag = vm_tag_will_update_zone(tag, z->tag_zone_index,
			    flags & (Z_WAITOK | Z_NOWAIT | Z_NOPAGEWAIT));
		}
		flags |= Z_VM_TAG(tag);
	}
#endif
	// Perform the memory allocation from the selected zone.
	addr = zalloc_ext(z, kheap->kh_stats ?: z->z_stats, flags);

	// Adjust the requested size to match the element size of the zone.
	req_size = zone_elem_size(z);

	/*
	 * Special handling for temporary heap allocations:
	 * Increment the temporary allocation counter for the current thread.
	 * If the counter exceeds a threshold, trigger a panic due to overuse.
	 */
	if (addr && kheap == KHEAP_TEMP) {
		thread_t self = current_thread();

		if (self->t_temp_alloc_count++ > UINT16_MAX) {
			kheap_temp_overuse_panic(self);
		}

		/*
		 * In DEBUG or DEVELOPMENT mode, record additional information
		    * about the temporary allocation for debugging purposes. This includes enqueuing the allocation in the
		* temporary allocation list and capturing a backtrace.
		*/
#if DEBUG || DEVELOPMENT
		if (__improbable(kheap_temp_debug)) {
			struct kheap_temp_header *hdr = (void )addr;
			enqueue_head(&self->t_temp_alloc_list, &hdr->kht_hdr_link);
			backtrace(hdr->kht_hdr_pcs, KHT_BT_COUNT, NULL);
			req_size -= sizeof(struct kheap_temp_header);
			addr += sizeof(struct kheap_temp_header);
		}
#endif / DEBUG || DEVELOPMENT */
	}
	// Record the allocation event using DTrace for performance monitoring and debugging.
	DTRACE_VM3(kalloc, vm_size_t, size, vm_size_t, req_size, void*, addr);

	// Return the result of the allocation including the address and the adjusted size.
	return (struct kalloc_result){ .addr = addr, .size = req_size };
}
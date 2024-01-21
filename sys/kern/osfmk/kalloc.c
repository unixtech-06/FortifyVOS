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

#include "../../arch/riscv32/include/_types.h"
#include "../../arch/riscv32/include/vm_types.h"

#include "../../sys/types.h"

#include "zalloc_internal.h"
#include "kalloc.h"

#define SECURITY_READ_ONLY_LATE(_t)                                            \
	_t __security_const_late __attribute__((                               \
	    used)) /* Darwin-xnu libkern/libkern/section_keywords.h */

#define MAX_SIZE_ZDLUT ((KALLOC_DLUT_SIZE - 1) * KALLOC_MINALIGN)
#define INDEX_ZDLUT(size) (((size) + KALLOC_MINALIGN - 1) / KALLOC_MINALIGN)

SECURITY_READ_ONLY_LATE(vm_size_t) kalloc_max_prerounded;

static TUNABLE(bool, kheap_temp_debug, "kheap_temp_debug", false);

/*
 * The k_zone_cfg table defines the configuration of zones on various platforms.
 * The currently defined list of zones and their per-CPU caching behavior are as
 * follows
 *
 *     X:zone not present
 *     N:zone present no cpu-caching
 *     Y:zone present with cpu-caching
 *
 * Size       macOS(64-bit)       embedded(32-bit)    embedded(64-bit)
 *--------    ----------------    ----------------    ----------------
 *
 * 8          X                    Y                   X
 * 16         Y                    Y                   Y
 * 24         X                    Y                   X
 * 32         Y                    Y                   Y
 * 40         X                    Y                   X
 * 48         Y                    Y                   Y
 * 64         Y                    Y                   Y
 * 72         X                    Y                   X
 * 80         Y                    X                   Y
 * 88         X                    Y                   X
 * 96         Y                    X                   Y
 * 112        X                    Y                   X
 * 128        Y                    Y                   Y
 * 160        Y                    X                   Y
 * 192        Y                    Y                   Y
 * 224        Y                    X                   Y
 * 256        Y                    Y                   Y
 * 288        Y                    Y                   Y
 * 368        Y                    X                   Y
 * 384        X                    Y                   X
 * 400        Y                    X                   Y
 * 440        X                    Y                   X
 * 512        Y                    Y                   Y
 * 576        Y                    N                   N
 * 768        Y                    N                   N
 * 1024       Y                    Y                   Y
 * 1152       N                    N                   N
 * 1280       N                    N                   N
 * 1536       X                    N                   X
 * 1664       N                    X                   N
 * 2048       Y                    N                   N
 * 2128       X                    N                   X
 * 3072       X                    N                   X
 * 4096       Y                    N                   N
 * 6144       N                    N                   N
 * 8192       Y                    N                   N
 * 12288      N                    X                   X
 * 16384      N                    X                   N
 * 32768      X                    X                   N
 *
 */
struct kalloc_zone_cfg {
	bool kzc_caching;
	uint32_t kzc_size;
	const char *kzc_name;
};

zone_t
kalloc_heap_zone_for_size(kalloc_heap_t kheap, vm_size_t size)
{
	struct kheap_zones* khz = kheap->kh_zones;

	if (size < MAX_SIZE_ZDLUT)
	{
		uint32_t zindex = khz->dlut[INDEX_ZDLUT(size)];
		return khz->k_zone[zindex];
	}

	if (size < kalloc_max_prerounded)
	{
		uint32_t zindex = khz->k_zindex_start;
		while (khz->cfg[zindex].kzc_size < size)
		{
			zindex++;
		}
		assert(zindex < khz->max_k_zone);
		return khz->k_zone[zindex];
	}

	return ZONE_NULL;
}

struct kalloc_result kalloc_ext(kalloc_heap_t kheap, vm_size_t req_size,
				zalloc_flags_t flags,
				vm_allocation_site_t* site)
{
	vm_size_t size;
	void* addr;
	zone_t z;

	/*
	 * Check for debugging flags in DEBUG or DEVELOPMENT builds.
	 * If kheap_temp_debug is enabled and the heap is KHEAP_TEMP,
	 * adjust the requested size to account for the temporary header.
	 */
#if DEBUG || DEVELOPMENT
	if (__improbable(kheap_temp_debug))
	{
		if (kheap == KHEAP_TEMP)
		{
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
	if (__improbable(z == ZONE_NULL))
	{
		return kalloc_large(kheap, req_size, size, flags, site);
	}

	/*
	 * In KALLOC_DEBUG mode, ensure the requested size does not exceed
	 * the maximum element size of the selected zone. Trigger a panic if it
	 * does.
	 */
#ifdef KALLOC_DEBUG
	if (size > zone_elem_size(z))
	{
		panic("%s: z %p (%s%s) but requested size %lu", __func__, z,
		      kalloc_heap_names[kheap->kh_zones->heap_id], z->z_name,
		      (unsigned long)size);
	}
#endif
	// Assert that the requested size is within the limits of the zone's
	// element size.
	assert(size <= zone_elem_size(z));

	/*
	 * If VM tags are enabled and the zone supports tagging, allocate
	 * a tag for this memory allocation and attach it to the flags.
	 */
#if VM_MAX_TAG_ZONES
	if (z->tags)
	{
		vm_tag_t tag = zalloc_flags_get_tag(flags);
		if (tag == VM_KERN_MEMORY_NONE && site)
		{
			tag = vm_tag_alloc(site);
		}
		if (tag != VM_KERN_MEMORY_NONE)
		{
			tag = vm_tag_will_update_zone(
			    tag, z->tag_zone_index,
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
	if (addr && kheap == KHEAP_TEMP)
	{
		thread_t self = current_thread();

		if (self->t_temp_alloc_count++ > UINT16_MAX)
		{
			kheap_temp_overuse_panic(self);
		}

		/*
		 * In DEBUG or DEVELOPMENT mode, record additional information
		 * about the temporary allocation for debugging purposes. This
		 * includes enqueuing the allocation in the temporary allocation
		 * list and capturing a backtrace.
		 */
#if DEBUG || DEVELOPMENT
		if (__improbable(kheap_temp_debug))
		{
			struct kheap_temp_header* hdr = (void)addr;
			enqueue_head(&self->t_temp_alloc_list,
				     &hdr->kht_hdr_link);
			backtrace(hdr->kht_hdr_pcs, KHT_BT_COUNT, NULL);
			req_size -= sizeof(struct kheap_temp_header);
			addr += sizeof(struct kheap_temp_header);
		}
#endif / DEBUG || DEVELOPMENT* /
	}
	// Record the allocation event using DTrace for performance monitoring
	// and debugging.
	DTRACE_VM3(kalloc, vm_size_t, size, vm_size_t, req_size, void*, addr);

	// Return the result of the allocation including the address and the
	// adjusted size.
	return (struct kalloc_result){.addr = addr, .size = req_size};
}

static void
kfree_ext(kalloc_heap_t kheap, void* data, vm_size_t size)
{
	zone_stats_t zs = NULL;
	zone_t z;
	vm_size_t zsize;

	if (__improbable(data == NULL))
	{
		return;
	}

	if (kheap == KHEAP_TEMP)
	{
		assert(size != KFREE_UNKNOWN_SIZE);
		if (current_thread()->t_temp_alloc_count-- == 0)
		{
			kfree_temp_imbalance_panic(data, size);
		}
#if DEBUG || DEVELOPMENT
		if (__improbable(kheap_temp_debug))
		{
			size += sizeof(struct kheap_temp_header);
			data -= sizeof(struct kheap_temp_header);
			remqueue(
			    &((struct kheap_temp_header*)data)->kht_hdr_link);
		}
#endif /* DEBUG || DEVELOPMENT */
	}

#if KASAN_KALLOC
	/*
	 * Resize back to the real allocation size and hand off to the KASan
	 * quarantine. `data` may then point to a different allocation.
	 */
	vm_size_t user_size = size;
	if (size == KFREE_UNKNOWN_SIZE)
	{
		user_size = size = kalloc_size(data);
	}
	kasan_check_free((vm_address_t)data, size, KASAN_HEAP_KALLOC);
	data = (void*)kasan_dealloc((vm_address_t)data, &size);
	kasan_free(&data, &size, KASAN_HEAP_KALLOC, NULL, user_size, true);
	if (!data)
	{
		return;
	}
#endif

	if (size >= kalloc_max_prerounded && size != KFREE_UNKNOWN_SIZE)
	{
		return kfree_large((vm_offset_t)data, size);
	}

	zsize = zone_element_size(data, &z);
	if (size == KFREE_UNKNOWN_SIZE)
	{
		if (zsize == 0)
		{
			return kfree_large((vm_offset_t)data, 0);
		}
		size = zsize;
	}
	else if (size > zsize)
	{
		kfree_size_confusion_panic(z, data, size, zsize);
	}

	if (kheap != KHEAP_ANY)
	{
		if (kheap->kh_heap_id != z->kalloc_heap)
		{
			kfree_heap_confusion_panic(kheap, data, size, z);
		}
		zs = kheap->kh_stats;
	}
	else if (z->kalloc_heap != KHEAP_ID_DEFAULT &&
		 z->kalloc_heap != KHEAP_ID_KEXT)
	{
		kfree_heap_confusion_panic(kheap, data, size, z);
	}

#if !KASAN_KALLOC
	DTRACE_VM3(kfree, vm_size_t, size, vm_size_t, zsize, void*, data);
#endif
	zfree_ext(z, zs ?: z->z_stats, data);
}

void
(kheap_free_addr)(kalloc_heap_t kheap, void* addr)
{
	kfree_ext(kheap, addr, KFREE_UNKNOWN_SIZE);
}
/*
 * Copyright (c) 2000-2020 Apple Inc. All rights reserved.
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

#ifndef _KERN_ZALLOC_INTERNAL_H_
#define _KERN_ZALLOC_INTERNAL_H_
#include "../../sys/types.h"
#include "zalloc.h"

#define KALLOC_MINALIGN     (1 << KALLOC_LOG2_MINALIGN)
#define KALLOC_DLUT_SIZE    (2048 / KALLOC_MINALIGN)
/*!
 * @struct zone_stats
 *
 * @abstract
 * Per-cpu structure used for basic zone stats.
 *
 * @discussion
 * The values aren't scaled for per-cpu zones.
 */
struct zone_stats
{
	uint64_t zs_mem_allocated;
	uint64_t zs_mem_freed;
	uint32_t zs_poison_seqno; /* counter for poisoning every N frees */
	uint32_t zs_alloc_rr;	  /* allocation rr bias */
};

struct zone
{
	/*
	 * Readonly / rarely written fields
	 */

	/*
	 * The first 4 fields match a zone_view.
	 *
	 * z_self points back to the zone when the zone is initialized,
	 * or is NULL else.
	 */
	struct zone* z_self;
	zone_stats_t z_stats;
	const char* z_name;
	struct zone_view* z_views;

	struct thread* z_expander;
	struct zone_cache* __zpercpu z_pcpu_cache;

	uint16_t z_chunk_pages; /* size used for more memory in pages  */
	uint16_t z_chunk_elems; /* count of allocations per chunk */
	uint16_t z_elems_rsv;	/* maintain a free reserve of elements */
	uint16_t z_elem_size;	/* size of an element                  */

	uint64_t
	    /*
	     * Lifecycle state (Mutable after creation)
	     */
	    z_destroyed : 1,	   /* zone is (being) destroyed */
	    z_async_refilling : 1, /* asynchronous allocation pending? */
	    z_replenish_wait : 1,  /* someone is waiting on the replenish thread
				    */
	    z_expanding_wait : 1,  /* is thread waiting for expansion? */
	    z_expander_vm_priv : 1, /* a vm privileged thread is expanding */

	    /*
	     * Security sensitive configuration bits
	     */
	    z_allows_foreign : 1, /* allow non-zalloc space  */
	    z_destructible : 1,	  /* zone can be zdestroy()ed  */
	    kalloc_heap : 2,	/* zone_kheap_id_t when part of a kalloc heap */
	    z_noencrypt : 1,	/* do not encrypt pages when hibernating */
	    z_submap_idx : 2,	/* a Z_SUBMAP_IDX_* value */
	    z_va_sequester : 1, /* page sequester: no VA reuse with other zones
				 */
	    z_free_zeroes : 1, /* clear memory of elements on free and assert on
				  alloc */

	    /*
	     * Behavior configuration bits
	     */
	    z_percpu : 1,      /* the zone is percpu */
	    z_permanent : 1,   /* the zone allocations are permanent */
	    z_replenishes : 1, /* uses the async replenish mechanism for VM */
	    z_nocaching : 1,   /* disallow zone caching for this zone */
	    collectable : 1,   /* garbage collect empty pages */
	    exhaustible : 1,   /* merely return if empty? */
	    expandable : 1,    /* expand zone (with message)? */
	    no_callout : 1,

	    _reserved : 26,

	    /*
	     * Debugging features
	     */
	    alignment_required : 1, /* element alignment needs to be preserved
				     */
	    gzalloc_tracked : 1,    /* this zone is tracked by gzalloc */
	    gzalloc_exempt : 1, /* this zone doesn't participate with gzalloc */
	    kasan_fakestacks : 1,
	    kasan_noquarantine : 1, /* whether to use the kasan quarantine */
	    tag_zone_index : 7, tags : 1, tags_inline : 1,
	    zleak_on : 1,     /* Are we collecting allocation information? */
	    zone_logging : 1; /* Enable zone logging for this zone. */

	/*
	 * often mutated fields
	 */

	lck_spin_t z_lock;
	struct zone_depot z_recirc;

	/*
	 * Page accounting (wired / VA)
	 *
	 * Those numbers are unscaled for z_percpu zones
	 * (zone_scale_for_percpu() needs to be used to find the true value).
	 */
	uint32_t z_wired_max;	/* how large can this zone grow        */
	uint32_t z_wired_hwm;	/* z_wired_cur high watermark          */
	uint32_t z_wired_cur;	/* number of pages used by this zone   */
	uint32_t z_wired_empty; /* pages collectable by GC             */
	uint32_t z_va_cur;	/* amount of VA used by this zone      */

	/*
	 * list of metadata structs, which maintain per-page free element lists
	 *
	 * Note: Due to the index packing in page metadata,
	 *       these pointers can't be at the beginning of the zone struct.
	 */
	zone_pva_t z_pageq_empty;   /* populated, completely empty pages   */
	zone_pva_t z_pageq_partial; /* populated, partially filled pages   */
	zone_pva_t z_pageq_full;    /* populated, completely full pages    */
	zone_pva_t z_pageq_va;	    /* non-populated VA pages              */

	/*
	 * Zone statistics
	 *
	 * z_contention_wma:
	 *   weighted moving average of the number of contentions per second,
	 *   in Z_CONTENTION_WMA_UNIT units (fixed point decimal).
	 *
	 * z_contention_cur:
	 *   count of recorded contentions that will be fused in
	 * z_contention_wma at the next period.
	 *
	 * z_recirc_cur:
	 *   number of magazines in the recirculation depot.
	 *
	 * z_elems_free:
	 *   number of free elements in the zone.
	 *
	 * z_elems_{min,max}:
	 *   tracks the low/high watermark of z_elems_free for the current
	 *   weighted moving average period.
	 *
	 * z_elems_free_wss:
	 *   weighted moving average of the (z_elems_free_max -
	 * z_elems_free_min) amplited which is used by the GC for trim
	 * operations.
	 *
	 * z_elems_avail:
	 *   number of elements in the zone (at all).
	 */
#define Z_CONTENTION_WMA_UNIT (1u << 8)
	uint32_t z_contention_wma;
	uint32_t z_contention_cur;
	uint32_t z_recirc_cur;
	uint32_t z_elems_free_max;
	uint32_t z_elems_free_wss;
	uint32_t z_elems_free_min;
	uint32_t z_elems_free;	/* Number of free elements             */
	uint32_t z_elems_avail; /* Number of elements available        */

#if CONFIG_ZLEAKS
	uint32_t zleak_capture; /* per-zone counter for capturing every N
				   allocations */
#endif
#if CONFIG_GZALLOC
	gzalloc_data_t gz;
#endif
#if KASAN_ZALLOC
	uint32_t z_kasan_redzone;
	spl_t z_kasan_spl;
#endif
#if DEBUG || DEVELOPMENT || CONFIG_ZLEAKS
	/* zone logging structure to hold stacks and element references to those
	 * stacks. */
	btlog_t* zlog_btlog;
#endif
};

#endif  /* _KERN_ZALLOC_INTERNAL_H_ */
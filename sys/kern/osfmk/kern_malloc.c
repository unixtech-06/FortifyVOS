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
/* Copyright (c) 1995, 1997 Apple Computer, Inc. All Rights Reserved */
/*
 * Copyright (c) 1987, 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)kern_malloc.c	8.4 (Berkeley) 5/20/95
 */
/*
 * NOTICE: This file was modified by SPARTA, Inc. in 2005 to introduce
 * support for mandatory and extensible security protections.  This notice
 * is included in support of clause 2.2 (b) of the Apple Public License,
 * Version 2.0.
 */
#include "assert.h"
#include "debug.h"

#include "kalloc.h"
#include "zalloc.h"

#include "../malloc.h"

#include "../../lib/libkern/libkern.h"
/*
 * __MALLOC_ext Function
 *
 * This function allocates memory. It can be used for different types of memory.
 * It also lets you choose how to handle the allocation.
 *
 * Parameters:
 *   size - How big the memory should be.
 *   type - The kind of memory allocation.
 *   flags - Special options for allocation (like waiting or not).
 *   site - Information about where the allocation is happening.
 *   heap - Where in memory to allocate from.
 *
 * Returns:
 *   A pointer to the memory, or NULL if we can't allocate.
 *
 * Note:
 *   If the type is wrong or size is zero, we return NULL.
 *   If we can't allocate memory and the flags say not to wait, we return NULL.
 *   If we can't allocate and the flags don't allow NULL, the program stops.
 */
static void*
__MALLOC_ext(size_t size, int type, int flags,
			  vm_allocation_site_t* site, kalloc_heap_t heap)
{
	void* addr = NULL;

	/* Check type and size ──> Invalid? ──> Panic */
	if (type >= M_LAST)
	{
		panic("_malloc TYPE");
	}

	if (size == 0)
	{
		return NULL;
	}

	/* Static assertions (not part of the flow chart but important for
	 * integrity) */
	static_assert(sizeof(vm_size_t) == sizeof(size_t));
	static_assert(M_WAITOK == Z_WAITOK);
	static_assert(M_NOWAIT == Z_NOWAIT);
	static_assert(M_ZERO == Z_ZERO);

	/* Allocate ──> Success? ──> Return address */
	addr = kalloc_ext(heap, size,
		flags & (M_WAITOK | M_NOWAIT | M_ZERO), site).addr;
	if (__probable(addr))
	{
		return addr;
	}

	/* Check flags ──> NOWAIT/NULL? ──> Return NULL */
	if (flags & (M_NOWAIT | M_NULL))
	{
		return NULL;
	}

	/*
	 * We get here when the caller told us to block waiting for memory, but
	 * kalloc said there's no memory left to get.  Generally, this means
	 * there's a leak or the caller asked for an impossibly large amount of
	 * memory. If the caller is expecting a NULL return code then it should
	 * explicitly set the flag M_NULL. If the caller isn't expecting a NULL
	 * return code, we just panic. This is less than ideal, but returning
	 * NULL when the caller isn't expecting it doesn't help since the
	 * majority of callers don't check the return value and will just
	 * dereference the pointer and trap anyway.  We may as well get a more
	 * descriptive message out while we can.
	 */
	panic("_MALLOC: kalloc returned NULL (potential leak), size %llu",
	      (uint64_t)size);
}

void*
__MALLOC(size_t size, int type, int flags, vm_allocation_site_t* site)
{
	return __MALLOC_ext(size, type, flags, site, KHEAP_DEFAULT);
}

void
_FREE_external(void* addr, int type);
void
_FREE_external(void* addr, int type __unused)
{
	/*
	 * hashinit and other functions allocate on behalf of kexts and do not
	 * have a matching hashdestroy, so we sadly have to allow this for now.
	 */
	kheap_free_addr(KHEAP_ANY, addr);
}
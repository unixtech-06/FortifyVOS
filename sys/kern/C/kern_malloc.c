/*	$EFortifyVOS: kern_malloc.c, Version: 2024, Last Maintenance: 2024/1/12 13:26 $	*/
/*
 * Copyright (c) 2023, Ryosuke
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: kern_malloc.c
 */

#include "../../arch/riscv32/include/_types.h"
#include "../../arch/riscv32/include/cpu.h"

#include "../../lib/libkern/libkern.h"

/*
*Declare the symbols __free_ram and __free_ram_end, which are defined within the linker script,
 *using extern char. Here, we are interested only in the addresses of these symbols, so we declare
 *them as char types for simplicity.
 */
extern char __free_ram[], __free_ram_end[];

/*
 * Allocate 'n' pages (where each page is 4096MB) using a static allocator.
 * Returns the physical address of the allocated memory.
 *
 * Parameters:
 * - __uint32_t n: Number of pages to allocate.
 *
 * Static Variables:
 * - static paddr_t next_paddr: Keeps track of the next available physical address
 *				for allocation. Initialized with the starting
 *				address of free RAM (__free_ram).
 *
 * Returns:
 * - paddr_t: Physical address of the allocated memory.
 *
 * Important Details:
 * - The allocator operates in units of pages, where each page has a size of 4048MB.
 * - The next available physical address is stored in 'next_paddr' and is updated
 *   after each allocation.
 * - A panic is triggered if the allocator runs out of memory, i.e., if the next
 *   available physical address exceeds the end address of free RAM (__free_ram_end).
 * - The allocated memory is zeroed using memset before returning the physical address.
*/
__paddr_t
alloc_pages(__uint32_t arg_n)
{
	__uint32_t n = arg_n;
	/* Static variable to keep track of the next available physical address. */
	static __paddr_t next_paddr = (__paddr_t) __free_ram;

	/* Save the current next_paddr as the allocated physical address. */
	__paddr_t paddr = next_paddr;

	/* Update next_paddr to point to the next available physical address. */
	next_paddr += n * PAGE_SIZE;

	/* Check for out-of-memory condition and trigger a panic if necessary. */
	if (next_paddr > (__paddr_t) __free_ram_end)
		PANIC("out of memory");

	/* Zero out the allocated memory. */
	memset((void *) paddr, 0, n * PAGE_SIZE); // PAGE_SIZE is 4048MB. cpu.h

	/* Return the physical address of the allocated memory. */
	return paddr;
}
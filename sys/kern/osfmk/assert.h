/*
 * Copyright (c) 2000-2016 Apple Inc. All rights reserved.
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

#ifndef _KERN_ASSERT_H_
#define _KERN_ASSERT_H_

#include "../../sys/cdefs.h"

#define assert(ex)  \
(__builtin_expect(!!((ex)), 1L) ? (void)0 : Assert(__FILE__, __LINE__, # ex))

/*
 * This header file provides a compatible implementation of the static_assert macro
 * for C11 standard and later. The static_assert macro is used for compile-time
 * assertions, to verify certain conditions during compilation.
 */

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
    /*
     * A helper macro to determine which static_assert version to use based on
     * the number of provided arguments. It selects the appropriate
     * static_assert implementation (one-argument or two-argument version).
     */
    #define _STATIC_ASSERT_OVERLOADED_MACRO(_1, _2, NAME, ...) NAME

    /*
     * The static_assert macro definition. It expands to either the one-argument
     * or two-argument version of static_assert, depending on the number of
     * arguments passed to it.
     */
    #define static_assert(...) _STATIC_ASSERT_OVERLOADED_MACRO(__VA_ARGS__, _static_assert_2_args, _static_assert_1_arg)(__VA_ARGS__)

    /*
     * Two-argument version of static_assert. This takes an expression and a
     * string literal as a message for the assertion failure.
     */
    #define _static_assert_2_args(ex, str) _Static_assert((ex), str)

    /*
     * One-argument version of static_assert. Only takes an expression and uses
     * the expression itself as the message if the assertion fails.
     */
    #define _static_assert_1_arg(ex) _Static_assert((ex), #ex)
#endif

#endif  /* _KERN_ASSERT_H_ */
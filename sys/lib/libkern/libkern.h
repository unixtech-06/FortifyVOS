/*	$EFortifyVOS: libkern.h, Version: 2024, Last Maintenance: 2024/1/9 14:30 $	*/
/*	$OpenBSD: libkern.h,v 1.37 2023/12/21 02:57:14 jsg Exp $	*/
/*	$NetBSD: libkern.h,v 1.7 1996/03/14 18:52:08 christos Exp $	*/

/*-
 * Copyright (c) 1992, 1993
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
 * 3. Neither the name of the University nor the names of its contributors
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
 *	@(#)libkern.h	8.1 (Berkeley) 6/10/93
 */
#ifndef __LIBKERN_H__
#define __LIBKERN_H__

#include "../../sys/types.h"

/* strlcpy.c */
size_t	 strlcpy(char *, const char *, int)
__attribute__ ((__bounded__(__string__,1,3)));

/* memset.c */
void *memset(void *buf, char c, size_t n);

/* memcpy.c */
void *memcpy(void *dst, const void *src, size_t n);

/* strcmp.c */
int strcmp(const char* s1, const char* s2);

#endif /* __LIBKERN_H__ */
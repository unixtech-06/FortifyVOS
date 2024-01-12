/*	$EFortifyVOS: cpu.h, Version: 2024, Last Maintenance: 2024/1/9 14:30 $	*/
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
 * File: cpu.h
 */

#pragma once
#include "_types.h"

#define PAGE_SIZE 4096 /* 1 mem page size */

struct trap_frame {
        __uint32_t ra;
        __uint32_t gp;
        __uint32_t tp;
        __uint32_t t0;
        __uint32_t t1;
        __uint32_t t2;
        __uint32_t t3;
        __uint32_t t4;
        __uint32_t t5;
        __uint32_t t6;
        __uint32_t a0;
        __uint32_t a1;
        __uint32_t a2;
        __uint32_t a3;
        __uint32_t a4;
        __uint32_t a5;
        __uint32_t a6;
        __uint32_t a7;
        __uint32_t s0;
        __uint32_t s1;
        __uint32_t s2;
        __uint32_t s3;
        __uint32_t s4;
        __uint32_t s5;
        __uint32_t s6;
        __uint32_t s7;
        __uint32_t s8;
        __uint32_t s9;
        __uint32_t s10;
        __uint32_t s11;
        __uint32_t sp;
} __attribute__((packed));

#define READ_CSR(reg)                                                          \
({                                                                         \
unsigned long __tmp;                                                   \
__asm__ __volatile__("csrr %0, " #reg : "=r"(__tmp));                  \
__tmp;                                                                 \
})

#define WRITE_CSR(reg, value)                                                  \
do {                                                                       \
__uint32_t __tmp = (value);                                              \
__asm__ __volatile__("csrw " #reg ", %0" ::"r"(__tmp));                \
} while (0)
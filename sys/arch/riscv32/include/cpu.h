#pragma once
#include "_types.h"

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
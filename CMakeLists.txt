cmake_minimum_required(VERSION 3.27)
project(MyKernel C)

set(CMAKE_C_STANDARD 11)

add_executable(MyKernel
        sys/arch/riscv32/include/_types.h
        sys/arch/riscv32/include/cpu.h
        sys/arch/riscv32/include/sbi.h
        sys/arch/riscv32/riscv32/kernel.c
        lib/libc/string/strcp.c
        sys/lib/libkern/memset.c
        sys/lib/libkern/memcpy.c
        sys/lib/libkern/strlcpy.c
        sys/lib/libkern/strcmp.c
        sys/arch/riscv32/riscv32/printf.c
        sys/kern/C/kern_malloc.c
        sys/kern/osfmk/assert.h
        sys/kern/osfmk/kalloc.c
        sys/kern/osfmk/zalloc.h
        sys/kern/osfmk/zalloc_internal.h
        sys/kern/osfmk/kern_types.h
        sys/kern/osfmk/kern_types.h
        sys/kern/osfmk/kern_types.h
        sys/kern/osfmk/kern_types.h
        sys/kern/osfmk/kern_malloc.c
        sys/kern/osfmk/zalloc.c
        sys/kern/osfmk/debug.h
        sys/kern/malloc.h
        sys/arch/riscv32/include/vm_types.h)

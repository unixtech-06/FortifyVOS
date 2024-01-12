/*	$EFortifyVOS: kernel.c, Version: 2024, Last Maintenance: 2024/1/9 14:30 $	*/
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
 * File: kernel.c
 */

#include "../include/_types.h"
#include "../include/sbi.h"
#include "../include/cpu.h"

#include "../../../lib/libkern/libkern.h"

/*
 * Define the boundaries of the bss section and the stack.
 */
extern int __bss[], __bss_end[], __stack_top[];

/*
 * Structure to hold the return values from an SBI call.
 */
struct sbiret sbi_call(long arg0, long arg1, long arg2, long arg3, long arg4,
                       long arg5, long fid, long eid) {
        register long a0 __asm__("a0") = arg0;
        register long a1 __asm__("a1") = arg1;
        register long a2 __asm__("a2") = arg2;
        register long a3 __asm__("a3") = arg3;
        register long a4 __asm__("a4") = arg4;
        register long a5 __asm__("a5") = arg5;
        register long a6 __asm__("a6") = fid;
        register long a7 __asm__("a7") = eid;

        /*
         * System call using the ecall instruction.
         * Inputs: a0-a7 registers
         * Outputs: a0 and a1 registers
         * Clobbers: memory
         */
        __asm__ __volatile__("ecall"
                : "=r"(a0), "=r"(a1)
                : "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5),
                "r"(a6), "r"(a7)
                : "memory");
        return (struct sbiret){.error = a0, .value = a1};
}

/*
 * System call to output a character to the console.
 */
void
putchar(int ch)
{
        sbi_call(ch, 0, 0, 0, 0, 0, 0, 1 /* Console Putchar */);
}

/*
 * Naked and aligned kernel entry point.
 */
__attribute__((naked))
__attribute__((aligned(4)))
void
kernel_entry(void)
{
        /*
         * Save the context on the stack.
         * This block saves registers ra, gp, tp, t0-t6, a0-a7, s0-s11, and sscratch.
         */
        __asm__ __volatile__(
                "csrw sscratch, sp\n"
                "addi sp, sp, -4 * 31\n"
                "sw ra,  4 * 0(sp)\n"
                "sw gp,  4 * 1(sp)\n"
                "sw tp,  4 * 2(sp)\n"
                "sw t0,  4 * 3(sp)\n"
                "sw t1,  4 * 4(sp)\n"
                "sw t2,  4 * 5(sp)\n"
                "sw t3,  4 * 6(sp)\n"
                "sw t4,  4 * 7(sp)\n"
                "sw t5,  4 * 8(sp)\n"
                "sw t6,  4 * 9(sp)\n"
                "sw a0,  4 * 10(sp)\n"
                "sw a1,  4 * 11(sp)\n"
                "sw a2,  4 * 12(sp)\n"
                "sw a3,  4 * 13(sp)\n"
                "sw a4,  4 * 14(sp)\n"
                "sw a5,  4 * 15(sp)\n"
                "sw a6,  4 * 16(sp)\n"
                "sw a7,  4 * 17(sp)\n"
                "sw s0,  4 * 18(sp)\n"
                "sw s1,  4 * 19(sp)\n"
                "sw s2,  4 * 20(sp)\n"
                "sw s3,  4 * 21(sp)\n"
                "sw s4,  4 * 22(sp)\n"
                "sw s5,  4 * 23(sp)\n"
                "sw s6,  4 * 24(sp)\n"
                "sw s7,  4 * 25(sp)\n"
                "sw s8,  4 * 26(sp)\n"
                "sw s9,  4 * 27(sp)\n"
                "sw s10, 4 * 28(sp)\n"
                "sw s11, 4 * 29(sp)\n"
        );

        /*
         * Set a0 register to the saved stack pointer.
         */
        __asm__ __volatile__("csrr a0, sscratch\n");

        /*
         * Call the trap handler.
         */
        __asm__ __volatile__("mv a0, sp\n"
                "call handle_trap\n");

        /*
         * Restore the context from the stack.
         * This block restores the saved registers.
         */
        __asm__ __volatile__(
                "lw ra,  4 * 0(sp)\n"
                "lw gp,  4 * 1(sp)\n"
                "lw tp,  4 * 2(sp)\n"
                "lw t0,  4 * 3(sp)\n"
                "lw t1,  4 * 4(sp)\n"
                "lw t2,  4 * 5(sp)\n"
                "lw t3,  4 * 6(sp)\n"
                "lw t4,  4 * 7(sp)\n"
                "lw t5,  4 * 8(sp)\n"
                "lw t6,  4 * 9(sp)\n"
                "lw a0,  4 * 10(sp)\n"
                "lw a1,  4 * 11(sp)\n"
                "lw a2,  4 * 12(sp)\n"
                "lw a3,  4 * 13(sp)\n"
                "lw a4,  4 * 14(sp)\n"
                "lw a5,  4 * 15(sp)\n"
                "lw a6,  4 * 16(sp)\n"
                "lw a7,  4 * 17(sp)\n"
                "lw s0,  4 * 18(sp)\n"
                "lw s1,  4 * 19(sp)\n"
                "lw s2,  4 * 20(sp)\n"
                "lw s3,  4 * 21(sp)\n"
                "lw s4,  4 * 22(sp)\n"
                "lw s5,  4 * 23(sp)\n"
                "lw s6,  4 * 24(sp)\n"
                "lw s7,  4 * 25(sp)\n"
                "lw s8,  4 * 26(sp)\n"
                "lw s9,  4 * 27(sp)\n"
                "lw s10, 4 * 28(sp)\n"
                "lw s11, 4 * 29(sp)\n"
        );

        /*
         * Return from trap.
         */
        __asm__ __volatile__("sret\n");
}

/*
 * Trap handler function.
 * The trap frame pointer is marked as unused.
 * Note: This attribute is temporary and used to suppress warnings about an unused parameter.
 */
void
handle_trap(struct trap_frame* f __attribute__((unused)))
{
        /*
         * Read trap cause, trap value, and user program counter.
         */
        __uint32_t scause = READ_CSR(scause);
        __uint32_t stval = READ_CSR(stval);
        __uint32_t user_pc = READ_CSR(sepc);

        /*
         * Panic with the trap details.
         */
        PANIC("unexpected trap scause=%x, stval=%x, sepc=%x\n", scause, stval, user_pc);
}

/*
 * Main kernel entry point.
 */
void
kernel_main(void)
{
        /*
         * Clear the bss section.
         */
        memset(__bss, 0, (__size_t) __bss_end - (__size_t) __bss);

        /*
         * Output a welcome message.
         */
        printf("Hello Wolrld!\n");

        /*
         * Set the trap vector to the kernel entry point.
         */
        WRITE_CSR(stvec, (__uint32_t) kernel_entry);

        /*
         * Trigger an invalid instruction to test trap handling.
         */
        __asm__ __volatile__("unimp"); // Invalid instruction

        /*
        // Alternative: Print a string using putchar and enter an infinite loop.
        const char *s = "\n\nHello World!\n";
        for (int i = 0; s[i] != '\0'; i++) {
            putchar(s[i]);
        }
        for (;;) {
            __asm__ __volatile__("wfi");
        }
        */
}

/*
 * Boot function to initialize stack and jump to kernel_main.
 */
__attribute__((section(".text.boot")))
__attribute__((naked))
void
boot(void)
{
        /*
         * Initialize the stack pointer and jump to kernel_main.
         */
        __asm__ __volatile__(
                "mv sp, %[stack_top]\n"
                "j kernel_main\n"
                :
                : [stack_top] "r" (__stack_top)
        );
}
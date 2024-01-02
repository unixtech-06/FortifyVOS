cmake_minimum_required(VERSION 3.10)
project(my_kernel_project C ASM)

set(QEMU qemu-system-riscv32)

# Set the path to clang (adjust accordingly)
set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm/bin/clang")
set(CMAKE_ASM_COMPILER "/opt/homebrew/opt/llvm/bin/clang")

# Compiler flags
set(CMAKE_C_FLAGS "-std=c11 -O2 -g3 -Wall -Wextra --target=riscv32 -ffreestanding -nostdlib")

# Kernel source files
set(KERNEL_SOURCE_FILES
        sys/arch/riscv32/riscv32/kernel.c
        sys/arch/riscv32/riscv32/common.c
        sys/arch/riscv32/include/_types.h
        sys/sys/_null.h
        sys/lib/libsa/memset.c
        sys/lib/libsa/memcpy.c
        lib/libc/string/strcp.c
        sys/lib/libsa/strcmp.c
        sys/sys/_types.h
        sys/arch/riscv32/include/sbi.h
        sys/lib/libkern/libkern.h
        sys/sys/types.h
        sys/sys/cdefs.h
        sys/arch/riscv32/include/cdefs.h
        sys/sys/endian.h
        sys/sys/_endian.h
        sys/arch/riscv32/include/endian.h
        sys/lib/libkern/strlcpy.h
        sys/lib/libkern/strlcpy.h
        # Add more source files if needed
)

# Kernel linker script
set(LINKER_SCRIPT sys/arch/riscv32/riscv32/kernel.ld)

# Output binary name
set(KERNEL_BINARY kernel.elf)

# Build the kernel
add_executable(${KERNEL_BINARY} ${KERNEL_SOURCE_FILES})
target_link_options(${KERNEL_BINARY} PRIVATE -Wl,-T${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_SCRIPT} -Wl,-Map=kernel.map)

# Custom target to run QEMU
add_custom_target(run
        COMMAND ${QEMU} -machine virt -bios default -nographic -serial mon:stdio --no-reboot -kernel ${KERNEL_BINARY}
        DEPENDS ${KERNEL_BINARY}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
#!/bin/bash
set -xue

QEMU=qemu-system-riscv32

# clangのパス (Ubuntuの場合は CC=clang)
CC=/opt/homebrew/opt/llvm/bin/clang

CFLAGS="-std=c11 -O2 -g3 -Wall -Wextra --target=riscv32 -ffreestanding -nostdlib"

# カーネルをビルド
# shellcheck disable=SC2086
$CC $CFLAGS -Wl,-T./sys/arch/riscv32/riscv32/kernel.ld -Wl,-Map=kernel.map -o kernel.elf \
    ./sys/arch/riscv32/riscv32/kernel.c ./sys/arch/riscv32/riscv32/common.c ./sys/lib/libsa/memset.c

# QEMUを起動
$QEMU -machine virt -bios default -nographic -serial mon:stdio --no-reboot \
    -kernel kernel.elf
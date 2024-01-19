const c = @cImport({
    @cInclude("../../arch/riscv32/include/_types.h");
    @cInclude("../../arch/riscv32/include/cpu.h");
    @cInclude("../../lib/libkern/libkern.h");
});

extern var __free_ram: [*]u8; // char型のポインタとして宣言
extern var __free_ram_end: [*]u8; // char型のポインタとして宣言

// カーネルの他の部分で定義されているPanic関数を仮定
extern fn Panic(msg: [*c]const u8) noreturn;

pub fn alloc_pages(n: c.__uint32_t) ?c.__paddr_t {
    const page_size = c.PAGE_SIZE; // cpu.hで定義されていると仮定

    var next_paddr = @ptrToInt(__free_ram); // 直接ポインタを使用
    const free_ram_end = @ptrToInt(__free_ram_end); // 直接ポインタを使用

    const paddr = next_paddr;

    next_paddr += n * page_size;

    // メモリ不足の状態をチェック
    if (next_paddr > free_ram_end) {
        Panic("out of memory");
    }

    // 割り当てられたメモリをゼロクリア
    var i: usize = 0;
    while (i < n * page_size) {
        __free_ram[i] = 0;
        i += 1;
    }

    return @intToPtr(c.__paddr_t, paddr);
}
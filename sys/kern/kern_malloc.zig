const c = @cImport({
        @cInclude("../arch/riscv32/include/_types.h");
        @cInclude("../arch/riscv32/include/cpu.h");
        @cInclude("../arch/riscv32/include/libkern.h");
});

const __uint32_t = u32; //* Do not use c.__uint32_t *//
const __paddr_t = u64; //* Do not use c.__paddr_t *//

var __free_ram: [*]u8 = undefined;
var __free_ram_end: [*]u8 = undefined;

pub export fn alloc_pages(arg_n: __uint32_t) __paddr_t {
        var n = arg_n;
        const next_paddr = struct {
                var static: __paddr_t = @as(__paddr_t, @intCast(@intFromPtr(@as([*]u8, @ptrCast(@alignCast(&__free_ram))))));
        };
        var paddr: __paddr_t = next_paddr.static;
        next_paddr.static +%= @as(__paddr_t, @bitCast(@as(c_ulong, n *% @as(__uint32_t, @bitCast(@as(c_int, 4096))))));
        if (next_paddr.static > @as(__paddr_t, @intCast(@intFromPtr(@as([*]u8, @ptrCast(@alignCast(&__free_ram_end))))))) while (true) {
                c.PANIC("out of memory");
        };
        _ = c.memset(@as(?*anyopaque, @ptrFromInt(paddr)), @as(u8, @bitCast(@as(i8, @truncate(@as(c_int, 0))))), @as(usize, @bitCast(@as(c_ulong, n *% @as(__uint32_t, @bitCast(@as(c_int, 4096)))))));
        return paddr;
}

pub fn main() !void { }
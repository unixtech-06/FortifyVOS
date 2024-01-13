pub const __builtin_bswap16 = @import("std").zig.c_builtins.__builtin_bswap16;
pub const __builtin_bswap32 = @import("std").zig.c_builtins.__builtin_bswap32;
pub const __builtin_bswap64 = @import("std").zig.c_builtins.__builtin_bswap64;
pub const __builtin_signbit = @import("std").zig.c_builtins.__builtin_signbit;
pub const __builtin_signbitf = @import("std").zig.c_builtins.__builtin_signbitf;
pub const __builtin_popcount = @import("std").zig.c_builtins.__builtin_popcount;
pub const __builtin_ctz = @import("std").zig.c_builtins.__builtin_ctz;
pub const __builtin_clz = @import("std").zig.c_builtins.__builtin_clz;
pub const __builtin_sqrt = @import("std").zig.c_builtins.__builtin_sqrt;
pub const __builtin_sqrtf = @import("std").zig.c_builtins.__builtin_sqrtf;
pub const __builtin_sin = @import("std").zig.c_builtins.__builtin_sin;
pub const __builtin_sinf = @import("std").zig.c_builtins.__builtin_sinf;
pub const __builtin_cos = @import("std").zig.c_builtins.__builtin_cos;
pub const __builtin_cosf = @import("std").zig.c_builtins.__builtin_cosf;
pub const __builtin_exp = @import("std").zig.c_builtins.__builtin_exp;
pub const __builtin_expf = @import("std").zig.c_builtins.__builtin_expf;
pub const __builtin_exp2 = @import("std").zig.c_builtins.__builtin_exp2;
pub const __builtin_exp2f = @import("std").zig.c_builtins.__builtin_exp2f;
pub const __builtin_log = @import("std").zig.c_builtins.__builtin_log;
pub const __builtin_logf = @import("std").zig.c_builtins.__builtin_logf;
pub const __builtin_log2 = @import("std").zig.c_builtins.__builtin_log2;
pub const __builtin_log2f = @import("std").zig.c_builtins.__builtin_log2f;
pub const __builtin_log10 = @import("std").zig.c_builtins.__builtin_log10;
pub const __builtin_log10f = @import("std").zig.c_builtins.__builtin_log10f;
pub const __builtin_abs = @import("std").zig.c_builtins.__builtin_abs;
pub const __builtin_fabs = @import("std").zig.c_builtins.__builtin_fabs;
pub const __builtin_fabsf = @import("std").zig.c_builtins.__builtin_fabsf;
pub const __builtin_floor = @import("std").zig.c_builtins.__builtin_floor;
pub const __builtin_floorf = @import("std").zig.c_builtins.__builtin_floorf;
pub const __builtin_ceil = @import("std").zig.c_builtins.__builtin_ceil;
pub const __builtin_ceilf = @import("std").zig.c_builtins.__builtin_ceilf;
pub const __builtin_trunc = @import("std").zig.c_builtins.__builtin_trunc;
pub const __builtin_truncf = @import("std").zig.c_builtins.__builtin_truncf;
pub const __builtin_round = @import("std").zig.c_builtins.__builtin_round;
pub const __builtin_roundf = @import("std").zig.c_builtins.__builtin_roundf;
pub const __builtin_strlen = @import("std").zig.c_builtins.__builtin_strlen;
pub const __builtin_strcmp = @import("std").zig.c_builtins.__builtin_strcmp;
pub const __builtin_object_size = @import("std").zig.c_builtins.__builtin_object_size;
pub const __builtin___memset_chk = @import("std").zig.c_builtins.__builtin___memset_chk;
pub const __builtin_memset = @import("std").zig.c_builtins.__builtin_memset;
pub const __builtin___memcpy_chk = @import("std").zig.c_builtins.__builtin___memcpy_chk;
pub const __builtin_memcpy = @import("std").zig.c_builtins.__builtin_memcpy;
pub const __builtin_expect = @import("std").zig.c_builtins.__builtin_expect;
pub const __builtin_nanf = @import("std").zig.c_builtins.__builtin_nanf;
pub const __builtin_huge_valf = @import("std").zig.c_builtins.__builtin_huge_valf;
pub const __builtin_inff = @import("std").zig.c_builtins.__builtin_inff;
pub const __builtin_isnan = @import("std").zig.c_builtins.__builtin_isnan;
pub const __builtin_isinf = @import("std").zig.c_builtins.__builtin_isinf;
pub const __builtin_isinf_sign = @import("std").zig.c_builtins.__builtin_isinf_sign;
pub const __has_builtin = @import("std").zig.c_builtins.__has_builtin;
pub const __builtin_assume = @import("std").zig.c_builtins.__builtin_assume;
pub const __builtin_unreachable = @import("std").zig.c_builtins.__builtin_unreachable;
pub const __builtin_constant_p = @import("std").zig.c_builtins.__builtin_constant_p;
pub const __builtin_mul_overflow = @import("std").zig.c_builtins.__builtin_mul_overflow;
pub const __int8_t = i8;
pub const __uint8_t = u8;
pub const __int16_t = c_short;
pub const __uint16_t = c_ushort;
pub const __int32_t = c_int;
pub const __uint32_t = c_uint;
pub const __int64_t = c_longlong;
pub const __uint64_t = c_ulonglong;
pub const __int_least8_t = __int8_t;
pub const __uint_least8_t = __uint8_t;
pub const __int_least16_t = __int16_t;
pub const __uint_least16_t = __uint16_t;
pub const __int_least32_t = __int32_t;
pub const __uint_least32_t = __uint32_t;
pub const __int_least64_t = __int64_t;
pub const __uint_least64_t = __uint64_t;
pub const __int_fast8_t = __int32_t;
pub const __uint_fast8_t = __uint32_t;
pub const __int_fast16_t = __int32_t;
pub const __uint_fast16_t = __uint32_t;
pub const __int_fast32_t = __int32_t;
pub const __uint_fast32_t = __uint32_t;
pub const __int_fast64_t = __int64_t;
pub const __uint_fast64_t = __uint64_t;
pub const __intptr_t = c_long;
pub const __uintptr_t = c_ulong;
pub const __intmax_t = __int64_t;
pub const __uintmax_t = __uint64_t;
pub const __register_t = c_long;
pub const __vaddr_t = c_ulong;
pub const __paddr_t = c_ulong;
pub const __vsize_t = c_ulong;
pub const __psize_t = c_ulong;
pub const __double_t = f64;
pub const __float_t = f32;
pub const __ptrdiff_t = c_long;
pub const __size_t = c_ulong;
pub const __ssize_t = c_long;
pub const __builtin_va_list = [*c]u8;
pub const __va_list = __builtin_va_list;
pub const __wchar_t = c_int;
pub const __wint_t = c_int;
pub const __rune_t = c_int;
pub const __wctrans_t = ?*anyopaque;
pub const __wctype_t = ?*anyopaque;
pub extern fn strcmp(s1: [*c]const u8, s2: [*c]const u8) c_int;
pub extern fn printf(fmt: [*c]const u8, ...) void;
pub const struct_trap_frame = extern struct {
    ra: __uint32_t align(1),
    gp: __uint32_t align(1),
    tp: __uint32_t align(1),
    t0: __uint32_t align(1),
    t1: __uint32_t align(1),
    t2: __uint32_t align(1),
    t3: __uint32_t align(1),
    t4: __uint32_t align(1),
    t5: __uint32_t align(1),
    t6: __uint32_t align(1),
    a0: __uint32_t align(1),
    a1: __uint32_t align(1),
    a2: __uint32_t align(1),
    a3: __uint32_t align(1),
    a4: __uint32_t align(1),
    a5: __uint32_t align(1),
    a6: __uint32_t align(1),
    a7: __uint32_t align(1),
    s0: __uint32_t align(1),
    s1: __uint32_t align(1),
    s2: __uint32_t align(1),
    s3: __uint32_t align(1),
    s4: __uint32_t align(1),
    s5: __uint32_t align(1),
    s6: __uint32_t align(1),
    s7: __uint32_t align(1),
    s8: __uint32_t align(1),
    s9: __uint32_t align(1),
    s10: __uint32_t align(1),
    s11: __uint32_t align(1),
    sp: __uint32_t align(1),
};
pub const __blkcnt_t = __int64_t;
pub const __blksize_t = __int32_t;
pub const __clock_t = __int64_t;
pub const __clockid_t = __int32_t;
pub const __cpuid_t = c_ulong;
pub const __dev_t = __int32_t;
pub const __fixpt_t = __uint32_t;
pub const __fsblkcnt_t = __uint64_t;
pub const __fsfilcnt_t = __uint64_t;
pub const __gid_t = __uint32_t;
pub const __id_t = __uint32_t;
pub const __in_addr_t = __uint32_t;
pub const __in_port_t = __uint16_t;
pub const __ino_t = __uint64_t;
pub const __key_t = c_long;
pub const __mode_t = __uint32_t;
pub const __nlink_t = __uint32_t;
pub const __off_t = __int64_t;
pub const __pid_t = __int32_t;
pub const __rlim_t = __uint64_t;
pub const __sa_family_t = __uint8_t;
pub const __segsz_t = __int32_t;
pub const __socklen_t = __uint32_t;
pub const __suseconds_t = c_long;
pub const __time_t = __int64_t;
pub const __timer_t = __int32_t;
pub const __uid_t = __uint32_t;
pub const __useconds_t = __uint32_t;
pub const __mbstate_t = extern union {
    __mbstate8: [128]u8,
    __mbstateL: __int64_t,
};
pub fn __swap16md(arg_x: __uint16_t) callconv(.C) __uint16_t {
    var x = arg_x;
    return @as(__uint16_t, @bitCast(@as(c_ushort, @truncate(((@as(c_uint, @bitCast(@as(c_uint, x))) & @as(c_uint, 255)) << @intCast(8)) | ((@as(c_uint, @bitCast(@as(c_uint, x))) & @as(c_uint, 65280)) >> @intCast(8))))));
}
pub fn __swap32md(arg_x: __uint32_t) callconv(.C) __uint32_t {
    var x = arg_x;
    return @as(__uint32_t, @bitCast(((((x & @as(__uint32_t, @bitCast(@as(c_int, 255)))) << @intCast(24)) | ((x & @as(__uint32_t, @bitCast(@as(c_int, 65280)))) << @intCast(8))) | ((x & @as(__uint32_t, @bitCast(@as(c_int, 16711680)))) >> @intCast(8))) | ((x & @as(c_uint, 4278190080)) >> @intCast(24))));
}
pub fn __swap64md(arg_x: __uint64_t) callconv(.C) __uint64_t {
    var x = arg_x;
    return @as(__uint64_t, @bitCast(((((((((x & @as(__uint64_t, @bitCast(@as(c_longlong, @as(c_int, 255))))) << @intCast(56)) | ((x & @as(c_ulonglong, 65280)) << @intCast(40))) | ((x & @as(c_ulonglong, 16711680)) << @intCast(24))) | ((x & @as(c_ulonglong, 4278190080)) << @intCast(8))) | ((x & @as(c_ulonglong, 1095216660480)) >> @intCast(8))) | ((x & @as(c_ulonglong, 280375465082880)) >> @intCast(24))) | ((x & @as(c_ulonglong, 71776119061217280)) >> @intCast(40))) | ((x & @as(c_ulonglong, 18374686479671623680)) >> @intCast(56))));
}
pub const u_char = u8;
pub const u_short = c_ushort;
pub const u_int = c_uint;
pub const u_long = c_ulong;
pub const unchar = u8;
pub const ushort = c_ushort;
pub const uint = c_uint;
pub const ulong = c_ulong;
pub const cpuid_t = __cpuid_t;
pub const register_t = __register_t;
pub const u_int8_t = __uint8_t;
pub const u_int16_t = __uint16_t;
pub const u_int32_t = __uint32_t;
pub const u_int64_t = __uint64_t;
pub const quad_t = __int64_t;
pub const u_quad_t = __uint64_t;
pub const vaddr_t = __vaddr_t;
pub const paddr_t = __paddr_t;
pub const vsize_t = __vsize_t;
pub const psize_t = __psize_t;
pub const blkcnt_t = __blkcnt_t;
pub const blksize_t = __blksize_t;
pub const caddr_t = [*c]u8;
pub const daddr32_t = __int32_t;
pub const daddr_t = __int64_t;
pub const dev_t = __dev_t;
pub const fixpt_t = __fixpt_t;
pub const gid_t = __gid_t;
pub const id_t = __id_t;
pub const ino_t = __ino_t;
pub const key_t = __key_t;
pub const mode_t = __mode_t;
pub const nlink_t = __nlink_t;
pub const rlim_t = __rlim_t;
pub const segsz_t = __segsz_t;
pub const uid_t = __uid_t;
pub const useconds_t = __useconds_t;
pub const suseconds_t = __suseconds_t;
pub const fsblkcnt_t = __fsblkcnt_t;
pub const fsfilcnt_t = __fsfilcnt_t;
pub const clock_t = __clock_t;
pub const clockid_t = __clockid_t;
pub const pid_t = __pid_t;
pub const time_t = __time_t;
pub const timer_t = __timer_t;
pub const off_t = __off_t;
pub extern fn lseek(c_int, off_t, c_int) off_t;
pub extern fn ftruncate(c_int, off_t) c_int;
pub extern fn truncate([*c]const u8, off_t) c_int;
pub extern fn strlcpy(dst: [*c]u8, src: [*c]const u8, dsize: c_ulong) c_ulong;
pub extern fn memset(buf: ?*anyopaque, c: u8, n: usize) ?*anyopaque;
pub extern fn memcpy(dst: ?*anyopaque, src: ?*const anyopaque, n: c_ulong) ?*anyopaque;
pub const __free_ram: [*c]u8 = @extern([*c]u8, .{
    .name = "__free_ram",
});
pub const __free_ram_end: [*c]u8 = @extern([*c]u8, .{
    .name = "__free_ram_end",
});
pub export fn alloc_pages(arg_n: __uint32_t) __paddr_t {
    var n = arg_n;
    const next_paddr = struct {
        var static: __paddr_t = @as(__paddr_t, @intCast(@intFromPtr(@as([*c]u8, @ptrCast(@alignCast(&__free_ram))))));
    };
    var paddr: __paddr_t = next_paddr.static;
    next_paddr.static +%= @as(__paddr_t, @bitCast(@as(c_ulong, n *% @as(__uint32_t, @bitCast(@as(c_int, 4096))))));
    if (next_paddr.static > @as(__paddr_t, @intCast(@intFromPtr(@as([*c]u8, @ptrCast(@alignCast(&__free_ram_end))))))) while (true) {
        printf("PANIC: %s:%d: out of memory\n", "kern_malloc.c", @as(c_int, 82));
        while (true) {}
        if (!false) break;
    };
    _ = memset(@as(?*anyopaque, @ptrFromInt(paddr)), @as(u8, @bitCast(@as(i8, @truncate(@as(c_int, 0))))), @as(usize, @bitCast(@as(c_ulong, n *% @as(__uint32_t, @bitCast(@as(c_int, 4096)))))));
    return paddr;
}
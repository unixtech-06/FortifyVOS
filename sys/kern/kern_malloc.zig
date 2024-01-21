const kern = @cImport({
    @cInclude("./osfmk/assert.h");
    @cInclude("./osfmk/debug.h");

    @cInclude("./osfmk/kalloc.h");
    @cInclude("./osfmk/zalloc.h");

    @cInclude("./malloc.h");

    @cInclude("../lib/libkern/libkern.h");
});

export
fn __MALLOC_ext(size: kern.size_t, kern_type: u32, flags: u32,
                       site: ?*kern.vm_allocation_site_, heap: ?*kern.kalloc_heap_t) ?*void
{
    var addr: ?*void = null;

    if (kern_type >= kern.M_LAST) {
        kern.panic("_malloc TYPE");
    }

    if (size == 0) {
        return null;
    }

    kern.static_assert(@sizeof(kern.vm_size_t) == @sizeof(kern.size_t));
    kern.static_assert(kern.M_WAITOK == kern.Z_WAITOK);
    kern.static_assert(kern.M_NOWAIT == kern.Z_NOWAIT);
    kern.static_assert(kern.M_ZERO == kern.Z_ZERO);

    addr = kern.kalloc_ext(heap, size, flags & (kern.M_WAITOK | kern.M_NOWAIT | kern.M_ZERO), site).addr;

    if (kern.__probable(addr)) {
        return addr;
    }

    if (flags & (kern.M_NOWAIT | kern.M_NULL)) {
        return null;
    }

    kern.panic("_MALLOC: kalloc returned NULL (potential leak), size %llu", @intCast(u64, size));
}

pub export
fn __MALLOC(size: kern.size_t, kern_type: u32, flags: u32,
                       site: ?*kern.vm_allocation_site_, heap: ?*kern.kalloc_heap_t) ?*void
{
    return __MALLOC_ext(size, kern_type, flags, site, heap);
}
#include "libkern.h"
#include "../../arch/riscv32/include/_types.h"

void *
memcpy(void *dst, const void *src, size_t n)
{
        __uint8_t *d = (__uint8_t *) dst;
        const __uint8_t *s = (const __uint8_t *) src;
        while (n--)
                *d++ = *s++;
        return dst;
}
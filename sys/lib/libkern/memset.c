#include "libkern.h"
#include "../../arch/riscv32/include/_types.h"

void *
memset(void *buf, char c, size_t n)
{
        __uint8_t *p = (__uint8_t *) buf;
        while (n--)
                *p++ = c;
        return buf;
}
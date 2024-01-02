#include "../../../sys/arch/riscv32/include/_types.h"

char *
strcpy(char *dst, const char *src)
{
        char *d = dst;
        while (*src)
                *d++ = *src++;
        *d = '\0';
        return dst;
}
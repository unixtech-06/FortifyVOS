#include "libkern.h"
#include "../../arch/riscv32/include/_types.h"

int
strcmp(const char* s1, const char* s2)
{
        while (*s1 && *s2) {
                if (*s1 != *s2)
                        break;
                s1++;
                s2++;
        }

        return *(unsigned char *) s1 - *(unsigned char *) s2;
}
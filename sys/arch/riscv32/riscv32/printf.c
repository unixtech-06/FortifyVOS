/*	$EFortifyVOS: common.c, Version: 2024, Last Maintenance: 2024/1/9 14:30 $	*/
/*
 * Copyright (c) 2023, Ryosuke
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: common.c
 */

#include "../include/_types.h"

void putchar(char ch);

/*
 * Custom printf function supporting %s, %d, and %x format specifiers.
 * Arguments:
 *   fmt: Format string with optional format specifiers.
 *   ...: Variable number of arguments corresponding to the format specifiers.
 */
void
printf(const char* fmt, ...)
{
        va_list vargs;
        va_start(vargs, fmt);

        while (*fmt) {
                if (*fmt == '%') {
                        fmt++;
                        switch (*fmt) {
                                case '\0':
                                        // Handle incomplete format specifier by printing '%'.
                                        putchar('%');
                                        goto end;
                                case '%':
                                        // Print '%' character for '%%' in the format.
                                        putchar('%');
                                        break;
                                case 's': {
                                        // Print a string (null-terminated character array).
                                        const char* s = va_arg(vargs, const char *);
                                        while (*s) {
                                                putchar(*s);
                                                s++;
                                        }
                                        break;
                                }
                                case 'd': {
                                        // Print a signed decimal integer.
                                        int value = va_arg(vargs, int);
                                        if (value < 0) {
                                                putchar('-');
                                                value = -value;
                                        }

                                        int divisor = 1;
                                        while (value / divisor > 9)
                                                divisor *= 10;

                                        while (divisor > 0) {
                                                putchar('0' + value / divisor);
                                                value %= divisor;
                                                divisor /= 10;
                                        }

                                        break;
                                }
                                case 'x': {
                                        // Print an unsigned hexadecimal integer.
                                        const int value = va_arg(vargs, int);
                                        for (int i = 7; i >= 0; i--) {
                                                const int nibble = (value >> (i * 4)) & 0xf;
                                                putchar("0123456789abcdef"[nibble]);
                                        }
                                        break;
                                }
                                default: ;
                        }
                } else {
                        // Print regular characters in the format string.
                        putchar(*fmt);
                }

                fmt++;
        }

end:
        va_end(vargs);
}

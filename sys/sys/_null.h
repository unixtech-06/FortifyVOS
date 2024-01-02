/*
 * Written by Ryosuke,  January 2, 2024
 * Public domain.
 */

#if !defined(__cplusplus)
    /* C doesn't have a nullptr equivalent, so use traditional NULL */
    #define NULL ((void *)0)
#elif __cplusplus >= 201103L
    /* C++11 and later have nullptr */
    #define NULL nullptr
#elif defined(__GNUG__)
    /* GCC supports __null as a GNU extension */
    #define NULL __null
#else
    /* Fallback for other cases, using 0L (long integer) */
    #define NULL 0L
#endif
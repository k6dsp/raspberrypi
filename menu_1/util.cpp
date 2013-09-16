#include "util.h"
#include <cstring>

static char digits[] =
        "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz";

/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 */
char* _itoa(int value, char * result, int base)
{
    // check that the base if valid
    if (base < 2 || base > 36)
    {
        *result = '\0';
        return result;
    }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do
    {
        tmp_value = value;
        value /= base;
        *ptr++ = digits[35 + (tmp_value - value * base)];
    } while (value);

    // Apply negative sign
    if (tmp_value < 0)
        *ptr++ = '-';
    *ptr-- = '\0';
    while (ptr1 < ptr)
    {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

bool _atoi(const char * x, int * result, int base)
{
    bool out = false;
    const char *lptr = x;
    *result = 0;
    int length = strlen((x));
    int char_offset = 0;
    while (length--)
    {
        char char_base = '0';
        if ((*lptr >= '0') && (*lptr <= '9'))
        {
            char_base = '0';
            char_offset = 0;
            out = true;
        }
        else
        {
            if ((*lptr >= 'A') && (*lptr <= 'F'))
            {
                char_base = 'A';
                char_offset = 10;
                out = true;
            }
            else
            {
                if ((*lptr >= 'a') && (*lptr <= 'f'))
                {
                    char_base = 'a';
                    char_offset = 10;
                    out = true;
                }
            }
        }
        *result *= base;
        *result += *lptr - char_base + char_offset;
        lptr++;
    }
    return true;
}

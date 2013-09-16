#ifndef UTIL_H_
#define UTIL_H_
#include <stdint.h>
#if defined (__cplusplus)
extern "C"
{
#endif // __cplusplus
#define abs(x)  (((x) > 0) ? (x): (-(x)))
#define itoa( a, b ) _itoa( ( a ),  ( b ), 10)
#define itoa16( a, b ) _itoa( ( a ), ( b ), 16 )
#define atoi( a, b ) _atoi( ( a ),  ( b ), 10)
#define atoi16( a, b ) _atoi( ( a ), ( b ), 16 )

    char* _itoa(int value, char * result, int base);
    bool _atoi(const char * x, int * result, int base);

#if defined (__cplusplus)
}
#endif // __cplusplus
#endif /* UTIL_H_ */

#ifndef __FLT_ARITHMETIC_H__
#define __FLT_ARITHMETIC_H__

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <immintrin.h>

// #define M_E         2.71828182845904523536028747135266250
#define AVX 1
#define SSE 0
#define CH  2 //chanel

typedef struct chanel_s{
    float ch[CH];
}chanel_t;



#if AVX
    typedef union flt_arithmetic
    {   
        float ch[4];
        __m256 i;
    }_float;
#elif SSE
    typedef __m128 my_float;
#else
    typedef chanel_t my_float
#endif

extern _float _add(union _float a, _float b);

extern _float _sub(_float a, _float b);

extern _float _mul(_float a, _float b);

extern _float _mac(_float a, _float b, _float c);

extern _float _msub(_float a, _float b, _float c);

extern _float _abs(_float a);

extern _float _neg(_float a);

extern _float _div(_float N, _float  D);

extern _float _pow2(_float n);


#endif /* colors.h */
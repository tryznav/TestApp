#ifndef __FLT_ARITHMETIC_H__
#define __FLT_ARITHMETIC_H__

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <immintrin.h>

// #define M_E         2.71828182845904523536028747135266250
#define AVX 0
#define SSE 0
#define CH  2 //chanel

typedef float mfloat;

#if AVX
    typedef union flt_arithmetic
    {   
        m_float ch[8];
        __m256 i;
    }_float;
#elif SSE
    typedef union flt_arithmetic
    {   
        m_float ch[4];
        __m128 i;
    }_float;
#else
    typedef struct vfloat_s{
        mfloat ch[CH];
    }vfloat;
#endif

extern vfloat vadd(vfloat a, vfloat b);

extern vfloat vsub(vfloat a, vfloat b);

extern vfloat vmul(vfloat a, vfloat b);

extern vfloat vmac(vfloat a, vfloat b, vfloat c);

extern vfloat vmsub(vfloat a, vfloat b, vfloat c);

extern vfloat vabs(vfloat a);

extern vfloat vneg(vfloat a);

extern vfloat vdiv(vfloat N, vfloat  D);

extern vfloat vpow2(vfloat n);


#endif /* colors.h */
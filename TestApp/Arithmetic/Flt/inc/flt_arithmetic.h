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
#define CH  8 //chanel



typedef float mfloat;

typedef struct stereo_s{
    mfloat Left;
    mfloat Right;
}stereo_t;

#if AVX

    typedef union flt_arithmetic
    {   
        mfloat ch[CH];
        __m256 i;
    }vfloat;
#elif SSE
    typedef union flt_arithmetic
    {   
        m_float ch[4];
        __m128 i;
    }vfloat;
#else
    // #define CH  2 //chanel
    typedef union vfloat_s{
        mfloat ch[CH];
        __m256 i;
    }vfloat;

    // typedef flt_arithmetic{
    //     mfloat      ch[CH];
    // }vfloat;
#endif

static inline vfloat vadd(vfloat a, vfloat b);

static inline vfloat vsub(vfloat a, vfloat b);

static inline vfloat vmul(vfloat a, vfloat b);

static inline vfloat vmac(vfloat a, vfloat b, vfloat c);

static inline vfloat vmsub(vfloat a, vfloat b, vfloat c);

static inline vfloat vabs(vfloat a);

static inline vfloat vneg(vfloat a);

static inline vfloat vdiv(vfloat N, vfloat  D);

static inline vfloat vpow2(vfloat n);

static inline vfloat vcpm_gt(vfloat a, vfloat b);

static inline vfloat vcpm_lt(vfloat a, vfloat b);

static inline vfloat vblend(vfloat a, vfloat b, vfloat mask);

#endif /* colors.h */
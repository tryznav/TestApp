#ifndef __FLT_ARITHMETIC_H__
#define __FLT_ARITHMETIC_H__

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <immintrin.h>
#include <stdlib.h>

#define INLINE      inline
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

#include "flt_arithmetic.h"

#define POW2EXP  0.075977616f
#define T2      1.882352948f
#define T1      2.823529482f

static INLINE vfloat vadd( vfloat a, vfloat b){
#if AVX
    vfloat res;
    res.i = _mm256_add_ps(a.i, b.i);
    return res;
#elif SSE
    return _mm128_add_ps(a.i, b.i);
#else
    vfloat res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = a.ch[i] + b.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat vsub(vfloat a, vfloat b){
#if AVX
    vfloat res;
    res.i = _mm256_sub_ps(a.i, b.i);
    return res;
#elif SSE
    
#else
    vfloat res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = a.ch[i] - b.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat vmul(vfloat a, vfloat b){
#if AVX
    vfloat res;
    res.i = _mm256_mul_ps(a.i, b.i);
    return res;
#elif SSE
    
#else
    vfloat res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = a.ch[i] * b.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat vmac(vfloat a, vfloat b, vfloat c){
#if AVX
    vfloat res;
    res.i = _mm256_fmadd_ps(a.i, b.i, c.i);
    return res;
#elif SSE

#else
    vfloat res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = (a.ch[i] * b.ch[i]) + c.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat vmsub(vfloat a, vfloat b, vfloat c){
#if AVX
    vfloat res;
    res.i = _mm256_fnmadd_ps(a.i, b.i, c.i);
    return res;
#elif SSE
    return _mm128_fnmadd_ps(a, b, c);
#else
    vfloat res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = -(a.ch[i] * b.ch[i]) + c.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat vabs(vfloat a){
#if AVX
    vfloat res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = (a.ch[i] < 0) ? -a.ch[i]: a.ch[i];
    }
    return res;
#elif SSE
    return _mm128_fnmadd_ps(a, b, c);
#else
    vfloat res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = (a.ch[i] < 0) ? -a.ch[i]: a.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat vneg(vfloat a){
#if AVX
    vfloat res;
    for(int i = 0; i < CH; i++){
        res.ch[i] =  -a.ch[i];
    }
    return res;
#elif SSE
    // return _mm128_fnmadd_ps(a, b, c);
#else
    vfloat res;
    for(int i = 0; i < CH; i++){
        res.ch[i] =  -a.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat vdiv(vfloat N, vfloat  D){
#if AVX
    vfloat res;
    res.i = _mm256_div_ps(N.i, D.i);
    return res;
#elif SSE
    return _mm128_fnmadd_ps(a, b, c);
#else
    vfloat res;
    for(int i = 0; i < CH; i++){
        res.ch[i] =  N.ch[i] / D.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat vpow2(vfloat n){
#if AVX
    vfloat res;
    for(int i = 0; i < CH; i++){
        res.ch[i] =  powf(n.ch[i], 2.0f);
    }
    return res;
#elif SSE
    return _mm128_fnmadd_ps(a, b, c);
#else
    vfloat res;
    for(int i = 0; i < CH; i++){
        res.ch[i] =  powf(n.ch[i], 2.0f);
    }
    return res;
#endif
}

static INLINE vfloat vpow(vfloat a, vfloat n){
#if AVX
    vfloat res;
    for(int i = 0; i < CH; i++){
        res.ch[i] =  powf(a.ch[i], n.ch[i]);
    }
    return res;
#elif SSE
    vfloat res;
    for(int i = 0; i < CH; i++){
        res.ch[i] =  powf(a.ch[i], n.ch[i]);
    }
    return res;
#else
    vfloat res;
    for(int i = 0; i < CH; i++){
        res.ch[i] =  powf(a.ch[i], n.ch[i]);
    }
    return res;
#endif
}

static INLINE vfloat vcpm_gt(vfloat a, vfloat b){
#if AVX
    vfloat res;
    res.i = _mm256_cmp_ps(a.i, b.i, _CMP_GT_OS);
    return res;
#elif SSE
    return _mm128_fnmadd_ps(a, b, c);
#else
    vfloat res;
    for(int i = 0; i < CH; i++){
        *((uint32_t *)&res.ch[i]) = (a.ch[i] > b.ch[i]) ? 0xFFFFFFFF : 0;
    }
    return res;
#endif
}


static INLINE vfloat vcpm_lt(vfloat a, vfloat b){
#if AVX
    vfloat res;
    res.i = _mm256_cmp_ps(a.i, b.i, _CMP_LT_OS);
    return res;
#elif SSE
    return _mm128_fnmadd_ps(a, b, c);
#else
    vfloat res;
    for(int i = 0; i < CH; i++){
        *((uint32_t *)&res.ch[i]) = (a.ch[i] < b.ch[i]) ? 0xFFFFFFFF : 0;
    }
    return res;
#endif
}

static INLINE vfloat vcpm_le(vfloat a, vfloat b){
#if AVX
    vfloat res;
    res.i = _mm256_cmp_ps(a.i, b.i, _CMP_LE_OS);
    return res;
#elif SSE
    return _mm128_fnmadd_ps(a, b, c);
#else
    vfloat res;
    for(int i = 0; i < CH; i++){
        *((uint32_t *)&res.ch[i]) = (a.ch[i] <= b.ch[i]) ? 0xFFFFFFFF : 0;
    }
    return res;
#endif
}

static INLINE vfloat vcpm_ge(vfloat a, vfloat b){
#if AVX
    vfloat res;
    res.i = _mm256_cmp_ps(a.i, b.i, _CMP_GE_OS);
    return  res;
#elif SSE
    return _mm128_fnmadd_ps(a, b, c);
#else
    vfloat res;
    for(int i = 0; i < CH; i++){
        *((uint32_t *)&res.ch[i]) = (a.ch[i] >= b.ch[i]) ? 0xFFFFFFFF : 0;
    }
    return res;
#endif
}

static INLINE vfloat vblend(vfloat a, vfloat b, vfloat mask){
#if AVX
    vfloat res;
    res.i = _mm256_blendv_ps(a.i, b.i, mask.i);
    return res;
#elif SSE

#else
    vfloat res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = (*((uint32_t *)&mask.ch[i])) ? b.ch[i] : a.ch[i];
    }
    return res;
#endif 
}

#endif /* colors.h */
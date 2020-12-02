#ifndef __FLT_ARITHMETIC_H__
#define __FLT_ARITHMETIC_H__

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <immintrin.h>
#include <stdlib.h>

#define INLINE      inline
#define AVX 1
#define SSE 1
#define CH  8 //chanel
#define EIGHT   8
#define FOUR    4


#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

typedef float mfloat;


#pragma pack(push, 2)   

    typedef union vfloat2_union{
        mfloat      ch[2];
        uint32_t    in[2];
    }vfloat2_t;

#if AVX
    typedef union vfloat8_union{
        mfloat      ch[EIGHT];
       __m256       i;
    }vfloat8_t;
#else
    typedef union vfloat8_union{
        mfloat      ch[EIGHT];
        uint32_t    in[EIGHT];
    }vfloat8_t;
#endif

#if SSE
    typedef union vfloat4_union{
        mfloat      ch[4];
        __m128      i;
    }vfloat4_t;
#else
    typedef union vfloat4_union{
        mfloat      ch[4];
        uint32_t    in[4];
    }vfloat4_t;
#endif

#pragma pack(pop)


#define POW2EXP  0.075977616f
#define T2      1.882352948f
#define T1      2.823529482f

static INLINE vfloat8_t v8_add( vfloat8_t a, vfloat8_t b){
#if AVX
    vfloat8_t res;
    res.i = _mm256_add_ps(a.i, b.i);
    return res;
#else
    vfloat8_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = a.ch[i] + b.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat8_t v8_sub(vfloat8_t a, vfloat8_t b){
#if AVX
    vfloat8_t res;
    res.i = _mm256_sub_ps(a.i, b.i);
    return res;
#else
    vfloat8_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = a.ch[i] - b.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat8_t v8_mul(vfloat8_t a, vfloat8_t b){
#if AVX
    vfloat8_t res;
    res.i = _mm256_mul_ps(a.i, b.i);
    return res;
#else
    vfloat8_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = a.ch[i] * b.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat8_t v8_mac(vfloat8_t a, vfloat8_t b, vfloat8_t c){
#if AVX
    vfloat8_t res;
    res.i = _mm256_fmadd_ps(a.i, b.i, c.i);
    return res;
#else
    vfloat8_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = (a.ch[i] * b.ch[i]) + c.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat8_t v8_msub(vfloat8_t a, vfloat8_t b, vfloat8_t c){
#if AVX
    vfloat8_t res;
    res.i = _mm256_fnmadd_ps(a.i, b.i, c.i);
    return res;
#else
    vfloat8_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = -(a.ch[i] * b.ch[i]) + c.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat8_t v8_abs(vfloat8_t a){
#if AVX
    vfloat8_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = (a.ch[i] < 0) ? -a.ch[i]: a.ch[i];
    }
    return res;
#else
    vfloat8_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = (a.ch[i] < 0) ? -a.ch[i]: a.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat8_t v8_neg(vfloat8_t a){
#if AVX
    vfloat8_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] =  -a.ch[i];
    }
    return res;
#else
    vfloat8_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] =  -a.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat8_t v8_div(vfloat8_t N, vfloat8_t  D){
#if AVX
    vfloat8_t res;
    res.i = _mm256_div_ps(N.i, D.i);
    return res;
#else
    vfloat8_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] =  N.ch[i] / D.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat8_t v8_pow2(vfloat8_t n){
#if AVX
    vfloat8_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] =  powf(n.ch[i], 2.0f);
    }
    return res;
#else
    vfloat8_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] =  powf(n.ch[i], 2.0f);
    }
    return res;
#endif
}

static INLINE vfloat8_t v8_pow(vfloat8_t a, vfloat8_t n){
#if AVX
    vfloat8_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] =  powf(a.ch[i], n.ch[i]);
    }
    return res;
#else
    vfloat8_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] =  powf(a.ch[i], n.ch[i]);
    }
    return res;
#endif
}

static INLINE vfloat8_t v8_cpm_gt(vfloat8_t a, vfloat8_t b){
#if AVX
    vfloat8_t res;
    res.i = _mm256_cmp_ps(a.i, b.i, _CMP_GT_OS);
    return res;
#else
    vfloat8_t res;
    for(int i = 0; i < CH; i++){
        // *((uint32_t *)&res.ch[i]) = (a.ch[i] > b.ch[i]) ? 0xFFFFFFFF : 0;
         res.in[i] = (a.ch[i] > b.ch[i]) ? 0xFFFFFFFF : 0;
    }
    return res;
#endif
}


static INLINE vfloat8_t v8_cpm_lt(vfloat8_t a, vfloat8_t b){
#if AVX
    vfloat8_t res;
    res.i = _mm256_cmp_ps(a.i, b.i, _CMP_LT_OS);
    return res;
#else
    vfloat8_t res;
    for(int i = 0; i < CH; i++){
        // *((uint32_t *)&res.ch[i]) = (a.ch[i] < b.ch[i]) ? 0xFFFFFFFF : 0;
        res.in[i] = (a.ch[i] < b.ch[i]) ? 0xFFFFFFFF : 0;
    }
    return res;
#endif
}

static INLINE vfloat8_t v8_cpm_le(vfloat8_t a, vfloat8_t b){
#if AVX
    vfloat8_t res;
    res.i = _mm256_cmp_ps(a.i, b.i, _CMP_LE_OS);
    return res;
#else
    vfloat8_t res;
    for(int i = 0; i < CH; i++){
         //*((uint32_t *)&res.ch[i]) = (a.ch[i] <= b.ch[i]) ? 0xFFFFFFFF : 0;
        res.in[i] = (a.ch[i] <= b.ch[i]) ? 0xFFFFFFFF : 0;
    }
    return res;
#endif
}

static INLINE vfloat8_t v8_cpm_ge(vfloat8_t a, vfloat8_t b){
#if AVX
    vfloat8_t res;
    res.i = _mm256_cmp_ps(a.i, b.i, _CMP_GE_OS);
    return  res;
#else
    vfloat8_t res;
    for(int i = 0; i < CH; i++){
        // *((uint32_t *)&res.ch[i]) = (a.ch[i] >= b.ch[i]) ? 0xFFFFFFFF : 0;
        res.in[i] = (a.ch[i] >= b.ch[i]) ? 0xFFFFFFFF : 0;
        // res.in[i] = (a.ch[i] >= b.ch[i]) ? 0xFFFFFFFF : 0;
    }
    return res;
#endif
}

static INLINE vfloat8_t v8_blend(vfloat8_t a, vfloat8_t b, vfloat8_t mask){
#if AVX
    vfloat8_t res;
    res.i = _mm256_blendv_ps(a.i, b.i, mask.i);
    return res;
#else
    vfloat8_t res;
    for(int i = 0; i < CH; i++){
        //res.ch[i] = (*((uint32_t *)&mask.ch[i])) ? b.ch[i] : a.ch[i];    //что-то непонятное 
        res.ch[i] = (mask.in[i]) ? b.ch[i] : a.ch[i];
    }
    return res;
#endif
}



//+++++++++++++++++++++++++++++++++++++++++++++++++

static INLINE vfloat4_t v4_add( vfloat4_t a, vfloat4_t b){
#if SSE
    vfloat4_t res;
    res.i = _mm_add_ps(a.i, b.i);
    return res;
#else
    vfloat4_t res;
    for(int i = 0; i < FOUR; i++){
        res.ch[i] = a.ch[i] + b.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat4_t v4_sub(vfloat4_t a, vfloat4_t b){
#if SSE
    vfloat4_t res;
    res.i = _mm_sub_ps(a.i, b.i);
    return res;
#else
    vfloat4_t res;
    for(int i = 0; i < FOUR; i++){
        res.ch[i] = a.ch[i] - b.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat4_t v4_mul(vfloat4_t a, vfloat4_t b){
#if SSE
    vfloat4_t res;
    res.i = _mm_mul_ps(a.i, b.i);
    return res;
#else
    vfloat4_t res;
    for(int i = 0; i < FOUR; i++){
        res.ch[i] = a.ch[i] * b.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat4_t v4_mac(vfloat4_t a, vfloat4_t b, vfloat4_t c){
#if SSE
    vfloat4_t res;
    res.i = _mm_fmadd_ps(a.i, b.i, c.i);
    return res;
#else
    vfloat4_t res;
    for(int i = 0; i < FOUR; i++){
        res.ch[i] = (a.ch[i] * b.ch[i]) + c.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat4_t v4_msub(vfloat4_t a, vfloat4_t b, vfloat4_t c){
#if SSE
    vfloat4_t res;
    res.i = _mm_fnmadd_ps(a.i, b.i, c.i);
    return res;
#else
    vfloat4_t res;
    for(int i = 0; i < FOUR; i++){
        res.ch[i] = -(a.ch[i] * b.ch[i]) + c.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat4_t v4_abs(vfloat4_t a){
#if SSE
    vfloat4_t res;
    for(int i = 0; i < FOUR; i++){
        res.ch[i] = (a.ch[i] < 0) ? -a.ch[i]: a.ch[i];
    }
    return res;
#else
    vfloat4_t res;
    for(int i = 0; i < FOUR; i++){
        res.ch[i] = (a.ch[i] < 0) ? -a.ch[i]: a.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat4_t v4_neg(vfloat4_t a){
#if SSE
    vfloat4_t res;
    for(int i = 0; i <  FOUR; i++){
        res.ch[i] =  -a.ch[i];
    }
    return res;
#else
    vfloat4_t res;
    for(int i = 0; i < FOUR; i++){
        res.ch[i] =  -a.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat4_t v4_div(vfloat4_t N, vfloat4_t  D){
#if SSE
    vfloat4_t res;
    res.i = _mm_div_ps(N.i, D.i);
    return res;
#else
    vfloat4_t res;
    for(int i = 0; i < FOUR; i++){
        res.ch[i] =  N.ch[i] / D.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat4_t v4_pow2(vfloat4_t n){
#if SSE
    vfloat4_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] =  powf(n.ch[i], 2.0f);
    }
    return res;
#else
    vfloat4_t res;
    for(int i = 0; i < FOUR; i++){
        res.ch[i] =  powf(n.ch[i], 2.0f);
    }
    return res;
#endif
}

static INLINE vfloat4_t v4_pow(vfloat4_t a, vfloat4_t n){
#if SSE
    vfloat4_t res;
    for(int i = 0; i < FOUR; i++){
        res.ch[i] =  powf(a.ch[i], n.ch[i]);
    }
    return res;
#else
    vfloat4_t res;
    for(int i = 0; i < FOUR; i++){
        res.ch[i] =  powf(a.ch[i], n.ch[i]);
    }
    return res;
#endif
}

static INLINE vfloat4_t v4_cpm_gt(vfloat4_t a, vfloat4_t b){
#if SSE
    vfloat4_t res;
    res.i = _mm_cmp_ps(a.i, b.i, _CMP_GT_OS);
    return res;
#else
    vfloat4_t res;
    for(int i = 0; i < FOUR; i++){
        // *((uint32_t *)&res.ch[i]) = (a.ch[i] > b.ch[i]) ? 0xFFFFFFFF : 0;
         res.in[i] = (a.ch[i] > b.ch[i]) ? 0xFFFFFFFF : 0;
    }
    return res;
#endif
}


static INLINE vfloat4_t v4_cpm_lt(vfloat4_t a, vfloat4_t b){
#if SSE
    vfloat4_t res;
    res.i = _mm_cmp_ps(a.i, b.i, _CMP_LT_OS);
    return res;
#else
    vfloat4_t res;
    for(int i = 0; i < FOUR; i++){
        // *((uint32_t *)&res.ch[i]) = (a.ch[i] < b.ch[i]) ? 0xFFFFFFFF : 0;
        res.in[i] = (a.ch[i] < b.ch[i]) ? 0xFFFFFFFF : 0;
    }
    return res;
#endif
}

static INLINE vfloat4_t v4_cpm_le(vfloat4_t a, vfloat4_t b){
#if SSE
    vfloat4_t res;
    res.i = _mm_cmp_ps(a.i, b.i, _CMP_LE_OS);
    return res;
#else
    vfloat4_t res;
    for(int i = 0; i < FOUR; i++){
         //*((uint32_t *)&res.ch[i]) = (a.ch[i] <= b.ch[i]) ? 0xFFFFFFFF : 0;
        res.in[i] = (a.ch[i] <= b.ch[i]) ? 0xFFFFFFFF : 0;
    }
    return res;
#endif
}

static INLINE vfloat4_t v4_cpm_ge(vfloat4_t a, vfloat4_t b){
#if SSE
    vfloat4_t res;
    res.i = _mm_cmp_ps(a.i, b.i, _CMP_GE_OS);
    return  res;
#else
    vfloat4_t res;
    for(int i = 0; i < FOUR; i++){
        // *((uint32_t *)&res.ch[i]) = (a.ch[i] >= b.ch[i]) ? 0xFFFFFFFF : 0;
        res.in[i] = (a.ch[i] >= b.ch[i]) ? 0xFFFFFFFF : 0;
        // res.in[i] = (a.ch[i] >= b.ch[i]) ? 0xFFFFFFFF : 0;
    }
    return res;
#endif
}

static INLINE vfloat4_t v4_blend(vfloat4_t a, vfloat4_t b, vfloat4_t mask){
#if SSE
    vfloat4_t res;
    res.i = _mm_blendv_ps(a.i, b.i, mask.i);
    return res;
#else
    vfloat4_t res;
    for(int i = 0; i < FOUR; i++){
        //res.ch[i] = (*((uint32_t *)&mask.ch[i])) ? b.ch[i] : a.ch[i];    //что-то непонятное 
        res.ch[i] = (mask.in[i]) ? b.ch[i] : a.ch[i];
    }
    return res;
#endif
}
#endif

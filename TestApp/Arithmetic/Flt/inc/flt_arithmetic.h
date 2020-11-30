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
#define EIGHT   8
#define FOUR    4


#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

typedef float mfloat;



#if AVX
// #pragma pack(push)  /* push current alignment to stack */
#pragma pack(push, 2)     /* set alignment to 2 byte boundary */
    typedef union flt_arithmetic
    {   
        mfloat ch[CH];
        __m256 i;
    }vfloat8_t;
#pragma pack(pop)
#elif SSE
    typedef union flt_arithmetic
    {   
        m_float ch[4];
        __m128 i;
    }vfloat8_t;
#else
    // #define CH  2 //chanel
  #pragma pack(push, 2)   
    // typedef union flt_arithmetic{
    //     mfloat ch[CH];
    //     uint32_t in[CH];
    // }vfloat8_t;

    typedef union vfloat2_union{
        mfloat      ch[2];
        uint32_t    in[2];
    }vfloat2_t;

    typedef union vfloat4_union{
        mfloat      ch[4];
        uint32_t    in[4];
    }vfloat4_t;

    typedef union vfloat8_union{
        mfloat      ch[EIGHT];
        uint32_t    in[EIGHT];
    }vfloat8_t;

  #pragma pack(pop)
    // typedef flt_arithmetic{
    //     mfloat      ch[CH];
    // }vfloat8_t;
#endif //AVX



#define POW2EXP  0.075977616f
#define T2      1.882352948f
#define T1      2.823529482f

static INLINE vfloat8_t v8_add( vfloat8_t a, vfloat8_t b){
#if AVX
    vfloat8_t res;
    res.i = _mm256_add_ps(a.i, b.i);
    return res;
#elif SSE
    return _mm128_add_ps(a.i, b.i);
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
#elif SSE
    
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
#elif SSE
    
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
#elif SSE

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
#elif SSE
    return _mm128_fnmadd_ps(a, b, c);
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
#elif SSE
    return _mm128_fnmadd_ps(a, b, c);
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
#elif SSE
    // return _mm128_fnmadd_ps(a, b, c);
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
#elif SSE
    return _mm128_fnmadd_ps(a, b, c);
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
#elif SSE
    return _mm128_fnmadd_ps(a, b, c);
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
#elif SSE
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
#elif SSE
    return _mm128_fnmadd_ps(a, b, c);
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
#elif SSE
    return _mm128_fnmadd_ps(a, b, c);
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
#elif SSE
    return _mm128_fnmadd_ps(a, b, c);
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
#elif SSE
    return _mm128_fnmadd_ps(a, b, c);
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
#elif SSE

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

static INLINE vfloat8_t v4_add( vfloat8_t a, vfloat8_t b){
#if AVX
    vfloat8_t res;
    res.i = _mm256_add_ps(a.i, b.i);
    return res;
#elif SSE
    return _mm128_add_ps(a.i, b.i);
#else
    vfloat4_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = a.ch[i] + b.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat4_t v4_sub(vfloat4_t a, vfloat4_t b){
#if AVX
    vfloat4_t res;
    res.i = _mm256_sub_ps(a.i, b.i);
    return res;
#elif SSE
    
#else
    vfloat4_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = a.ch[i] - b.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat4_t v4_mul(vfloat4_t a, vfloat4_t b){
#if AVX
    vfloat4_t res;
    res.i = _mm256_mul_ps(a.i, b.i);
    return res;
#elif SSE
    
#else
    vfloat4_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = a.ch[i] * b.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat4_t v4_mac(vfloat4_t a, vfloat4_t b, vfloat4_t c){
#if AVX
    vfloat4_t res;
    res.i = _mm256_fmadd_ps(a.i, b.i, c.i);
    return res;
#elif SSE

#else
    vfloat4_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = (a.ch[i] * b.ch[i]) + c.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat4_t v4_msub(vfloat4_t a, vfloat4_t b, vfloat4_t c){
#if AVX
    vfloat4_t res;
    res.i = _mm256_fnmadd_ps(a.i, b.i, c.i);
    return res;
#elif SSE
    return _mm128_fnmadd_ps(a, b, c);
#else
    vfloat4_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = -(a.ch[i] * b.ch[i]) + c.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat4_t v4_abs(vfloat4_t a){
#if AVX
    vfloat4_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = (a.ch[i] < 0) ? -a.ch[i]: a.ch[i];
    }
    return res;
#elif SSE
    return _mm128_fnmadd_ps(a, b, c);
#else
    vfloat4_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = (a.ch[i] < 0) ? -a.ch[i]: a.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat4_t v4_neg(vfloat4_t a){
#if AVX
    vfloat4_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] =  -a.ch[i];
    }
    return res;
#elif SSE
    // return _mm128_fnmadd_ps(a, b, c);
#else
    vfloat4_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] =  -a.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat4_t v4_div(vfloat4_t N, vfloat4_t  D){
#if AVX
    vfloat4_t res;
    res.i = _mm256_div_ps(N.i, D.i);
    return res;
#elif SSE
    return _mm128_fnmadd_ps(a, b, c);
#else
    vfloat4_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] =  N.ch[i] / D.ch[i];
    }
    return res;
#endif
}

static INLINE vfloat4_t v4_pow2(vfloat4_t n){
#if AVX
    vfloat4_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] =  powf(n.ch[i], 2.0f);
    }
    return res;
#elif SSE
    return _mm128_fnmadd_ps(a, b, c);
#else
    vfloat4_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] =  powf(n.ch[i], 2.0f);
    }
    return res;
#endif
}

static INLINE vfloat4_t v4_pow(vfloat4_t a, vfloat4_t n){
#if AVX
    vfloat4_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] =  powf(a.ch[i], n.ch[i]);
    }
    return res;
#elif SSE
    vfloat4_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] =  powf(a.ch[i], n.ch[i]);
    }
    return res;
#else
    vfloat4_t res;
    for(int i = 0; i < CH; i++){
        res.ch[i] =  powf(a.ch[i], n.ch[i]);
    }
    return res;
#endif
}

static INLINE vfloat4_t v8_cpm_gt(vfloat4_t a, vfloat4_t b){
#if AVX
    vfloat4_t res;
    res.i = _mm256_cmp_ps(a.i, b.i, _CMP_GT_OS);
    return res;
#elif SSE
    return _mm128_fnmadd_ps(a, b, c);
#else
    vfloat4_t res;
    for(int i = 0; i < CH; i++){
        // *((uint32_t *)&res.ch[i]) = (a.ch[i] > b.ch[i]) ? 0xFFFFFFFF : 0;
         res.in[i] = (a.ch[i] > b.ch[i]) ? 0xFFFFFFFF : 0;
    }
    return res;
#endif
}


static INLINE vfloat4_t v8_cpm_lt(vfloat4_t a, vfloat4_t b){
#if AVX
    vfloat4_t res;
    res.i = _mm256_cmp_ps(a.i, b.i, _CMP_LT_OS);
    return res;
#elif SSE
    return _mm128_fnmadd_ps(a, b, c);
#else
    vfloat4_t res;
    for(int i = 0; i < CH; i++){
        // *((uint32_t *)&res.ch[i]) = (a.ch[i] < b.ch[i]) ? 0xFFFFFFFF : 0;
        res.in[i] = (a.ch[i] < b.ch[i]) ? 0xFFFFFFFF : 0;
    }
    return res;
#endif
}

static INLINE vfloat4_t v8_cpm_le(vfloat4_t a, vfloat4_t b){
#if AVX
    vfloat4_t res;
    res.i = _mm256_cmp_ps(a.i, b.i, _CMP_LE_OS);
    return res;
#elif SSE
    return _mm128_fnmadd_ps(a, b, c);
#else
    vfloat4_t res;
    for(int i = 0; i < CH; i++){
         //*((uint32_t *)&res.ch[i]) = (a.ch[i] <= b.ch[i]) ? 0xFFFFFFFF : 0;
        res.in[i] = (a.ch[i] <= b.ch[i]) ? 0xFFFFFFFF : 0;
    }
    return res;
#endif
}

static INLINE vfloat4_t v8_cpm_ge(vfloat4_t a, vfloat4_t b){
#if AVX
    vfloat4_t res;
    res.i = _mm256_cmp_ps(a.i, b.i, _CMP_GE_OS);
    return  res;
#elif SSE
    return _mm128_fnmadd_ps(a, b, c);
#else
    vfloat4_t res;
    for(int i = 0; i < CH; i++){
        // *((uint32_t *)&res.ch[i]) = (a.ch[i] >= b.ch[i]) ? 0xFFFFFFFF : 0;
        res.in[i] = (a.ch[i] >= b.ch[i]) ? 0xFFFFFFFF : 0;
        // res.in[i] = (a.ch[i] >= b.ch[i]) ? 0xFFFFFFFF : 0;
    }
    return res;
#endif
}

static INLINE vfloat4_t v8_blend(vfloat4_t a, vfloat4_t b, vfloat4_t mask){
#if AVX
    vfloat4_t res;
    res.i = _mm256_blendv_ps(a.i, b.i, mask.i);
    return res;
#elif SSE

#else
    vfloat4_t res;
    for(int i = 0; i < CH; i++){
        //res.ch[i] = (*((uint32_t *)&mask.ch[i])) ? b.ch[i] : a.ch[i];    //что-то непонятное 
        res.ch[i] = (mask.in[i]) ? b.ch[i] : a.ch[i];
    }
    return res;
#endif
}
#endif

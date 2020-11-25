#include "flt_arithmetic.h"

#define POW2EXP  0.075977616f
#define T2      1.882352948f
#define T1      2.823529482f

// #define _CMP_GT_OS    0x0e
// #define _CMP_LT_OS    0x01

static inline vfloat vadd( vfloat a, vfloat b){
#if AVX
    return _mm256_add_ps(a.i, b.i);
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

static inline vfloat vsub(vfloat a, vfloat b){
#if AVX
    return _mm256_sub_ps(a.i, b.i);
#elif SSE
    return _mm128_sub_ps(a.i, b.i);
#else
    vfloat res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = a.ch[i] - b.ch[i];
    }
    return res;
#endif
}

static inline vfloat vmul(vfloat a, vfloat b){
#if AVX
    return _mm256_sub_ps(a, b);
#elif SSE
    return _mm128_sub_ps(a, b);
#else
    vfloat res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = a.ch[i] * b.ch[i];
    }
    return res;
#endif
}

static inline vfloat vmac(vfloat a, vfloat b, vfloat c){
#if AVX
    return _mm256_sub_ps(a, b);
#elif SSE
    return _mm128_sub_ps(a, b);
#else
    vfloat res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = (a.ch[i] * b.ch[i]) + c.ch[i];
    }
    return res;
#endif
}

static inline vfloat vmsub(vfloat a, vfloat b, vfloat c){
#if AVX
    return _mm256_fnmadd_ps(a, b, c);
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

static inline vfloat vabs(vfloat a){
#if AVX
    return _mm256_fnmadd_ps(a);
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

static inline vfloat vneg(vfloat a){
#if AVX
    return _mm256_fnmadd_ps(a);
#elif SSE
    return _mm128_fnmadd_ps(a, b, c);
#else
    vfloat res;
    for(int i = 0; i < CH; i++){
        res.ch[i] =  -a.ch[i];
    }
    return res;
#endif
}

static inline vfloat vdiv(vfloat N, vfloat  D){
#if AVX
    return _mm256_fnmadd_ps(a);
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

static inline vfloat vpow2(vfloat n){
#if AVX
    return _mm256_fnmadd_ps(a);
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

static inline vfloat vcpm_gt(vfloat a, vfloat b){
#if AVX
    return _mm256_cmp_ps(a.i, b.i, _CMP_LT_OS);
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


static inline vfloat vcpm_lt(vfloat a, vfloat b){
#if AVX
    return _mm256_cmp_ps(a.i, b.i, _CMP_GT_OS);
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

static inline vfloat vblend(vfloat a, vfloat b, vfloat mask){
   #if AVX
    return _mm256_blendv_ps(a.i, b.i, mask.i);
#elif SSE
    return _mm128_fnmadd_ps(a, b, c);
#else
    vfloat res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = (*((uint32_t *)&res.ch[i])) ? b.ch[i] : a.ch[i];
    }
    return res;
#endif 
}

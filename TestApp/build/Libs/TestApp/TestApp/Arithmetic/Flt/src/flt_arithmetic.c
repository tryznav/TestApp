#include "flt_arithmetic.h"

#define POW2EXP  0.075977616f
#define T2      1.882352948f
#define T1      2.823529482f

inline vfloat vadd( vfloat a, vfloat b){
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

inline vfloat vsub(vfloat a, vfloat b){
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

inline vfloat vmul(vfloat a, vfloat b){
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

inline vfloat vmac(vfloat a, vfloat b, vfloat c){
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

inline vfloat vmsub(vfloat a, vfloat b, vfloat c){
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

inline vfloat vabs(vfloat a){
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

inline vfloat vneg(vfloat a){
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

inline vfloat vdiv(vfloat N, vfloat  D){
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

inline vfloat vpow2(vfloat n){
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
    return n;
}

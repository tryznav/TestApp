//flt_arithmetic.c

#include "flt_arithmetic.h"

#define POW2EXP  0.075977616f
#define T2      1.882352948f
#define T1      2.823529482f

inline _float _add( _float a, _float b){
#if AVX
    return _mm256_add_ps(a.i, b.i);
#elif SSE
    return _mm128_add_ps(a.i, b.i);
#else
    _float res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = a.ch[i] + b.ch[i];
    }
    return res;
#endif
}

inline _float _sub(_float a, _float b){
#if AVX
    return _mm256_sub_ps(a.i, b.i);
#elif SSE
    return _mm128_sub_ps(a.i, b.i);
#else
    _float res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = a.ch[i] - b.ch[i];
    }
    return res;
#endif
}

inline _float _mul(_float a, _float b){
#if AVX
    return _mm256_sub_ps(a, b);
#elif SSE
    return _mm128_sub_ps(a, b);
#else
    _float res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = a.ch[i] * b.ch[i];
    }
    return res;
#endif
}

inline _float _mac(_float a, _float b, _float c){
#if AVX
    return _mm256_sub_ps(a, b);
#elif SSE
    return _mm128_sub_ps(a, b);
#else
    _float res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = a.ch[i] * b.ch[i];
    }
    return res;
#endif
}

inline _float _msub(_float a, _float b, _float c){
#if AVX
    return _mm256_fnmadd_ps(a, b, c);
#elif SSE
    return _mm128_fnmadd_ps(a, b, c);
#else
    _float res;
    for(int i = 0; i < CH; i++){
        res.ch[i] = -(a.ch[i] * b.ch[i]) + c;
    }
    return res;
#endif
}

// inline _float _abs(_float a){
// #if AVX
//     return _mm256_fnmadd_ps(a);
// #elif SSE
//     return _mm128_fnmadd_ps(a, b, c);
// #else
//     _float res;
//     for(int i = 0; i < CH; i++){
//         res.ch[i] = -(a.ch[i] * b.ch[i]) + c;
//     }
//     return res;
// #endif
//     return (a < 0) ? -a : a;
// }

// inline _float _neg(_float a){
//     return -a;
// }

// inline _float _div(_float N, _float  D){

//     _float  Dsh = flt_abs(D) * POW2EXP; ///powf(2, M_E + 1);
//     _float  Nsh = flt_abs(N) * POW2EXP; //powf(2, M_E + 1);

//     _float x = T1 - T2 * Dsh;
//     for(int i = 0; i < 5; i++){
//         x = x + x*(1.0f - Dsh * x);
//     }
//     return x * Nsh;
// }

// inline _float   flt_pow2(_float n){
//     return n;
// }

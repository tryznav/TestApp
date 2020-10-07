//flt_arithmetic.c

#include "flt_arithmetic.h"

#define POW2EXP  0.075977616f
#define T2      1.882352948f
#define T1      2.823529482f

my_float   flt_add(my_float a, my_float b){
    return a + b;
}

my_float   flt_sub(my_float a, my_float b){
        return a - b;
}

my_float   flt_mul(my_float a, my_float b){
    return a * b;
}

my_float   flt_mac(my_float a, my_float b, my_float c){
        return a + (b * c);
}

my_float   flt_msub(my_float a, my_float b, my_float c){
        return a - (b * c);
}

my_float   flt_abs(my_float a){
        return (a < 0) ? -a : a;
}

my_float   flt_neg(my_float a){
        return -a;
}

my_float   flt_div(my_float N, my_float  D){

    my_float  Dsh = flt_abs(D) * POW2EXP; ///powf(2, M_E + 1);
    my_float  Nsh = flt_abs(N) * POW2EXP; //powf(2, M_E + 1);

    my_float x = T1 - T2 * Dsh;
    for(int i = 0; i < 5; i++){
        x = x + x*(1.0f - Dsh * x);
    }
    return x * Nsh;
}

my_float   flt_pow2(my_float n){
    return n;
}

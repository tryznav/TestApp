#ifndef __FLT_ARITHMETIC_H__
#define __FLT_ARITHMETIC_H__

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

// #define M_E         2.71828182845904523536028747135266250

typedef float my_float;

my_float   flt_add(my_float a, my_float b);

my_float   flt_sub(my_float a, my_float b);

my_float   flt_mul(my_float a, my_float b);

my_float   flt_mac(my_float a, my_float b, my_float c);

my_float   flt_msub(my_float a, my_float b, my_float c);

my_float   flt_abs(my_float a);

my_float   flt_neg(my_float a);

my_float   flt_div(my_float N, my_float  D);

my_float   flt_pow2(my_float);


#endif /* colors.h */
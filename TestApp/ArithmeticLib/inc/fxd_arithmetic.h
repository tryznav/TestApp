#ifndef __FXD_ARITHMETIC_H__
#define __FXD_ARITHMETIC_H__

#include "flt_arithmetic.h"

#include <assert.h>
#include <stdint.h>
#define FRACTION_BITS           31

#define HALF_Q31_PL 0x40000000

#define M_E         2.71828182845904523536028747135266250

#include <stdio.h>
#include <string.h>

typedef int32_t fxd_q31_t;
typedef int64_t fxd_q63_t;

typedef int32_t fxd_q5_26_t;

extern const fxd_q31_t POW2_FXD[512];
extern const fxd_q31_t LOG2_FXD[512];
extern const fxd_q5_26_t LOG2_FXD_Q27[513];
extern const fxd_q5_26_t POW2_FXD_Q26[513];
extern const int32_t POW_INVERT[513];

#define FRACTIONAL_BASE         (1u<<FRACTION_BITS)

#define FRACTIONAL_MAX          ((1ul<<FRACTION_BITS)-1)
#define FRACTIONAL_MIN          (-1<<FRACTION_BITS)

#define HALF_Q31_PL             0x40000000

#define INTERP_FACTOR_MASK_LOG  ((1 << 21) - 1)

/*******************************************************************************
 *  Saturates the data to the maximum values Q31
 * 
 * @param     number    64bit value
 * 
 * @return              saturated value
 ******************************************************************************/
fxd_q63_t   saturation(fxd_q63_t a);

/*******************************************************************************
 *  Addition(+) with saturation for format Q31(32 bit)
 * 
 * @param     a     addendum in format Q31(32bit)
 * @param     b     addendum in format Q31(32bit)
 * 
 * @return          sum in format Q31(32bit)
 ******************************************************************************/
fxd_q31_t   fxd_add(fxd_q31_t a, fxd_q31_t b);

/*******************************************************************************
 *  Subtraction(-) with saturation for format Q31(32 bit)
 * 
 * @param     a     minuend in format Q31(32bit) 
 * @param     b     subtrahend in format Q31(32bit)
 * 
 * @return          difference in format Q31(32bit)
 ******************************************************************************/
fxd_q31_t   fxd_sub(fxd_q31_t a, fxd_q31_t b);

/*******************************************************************************
 *  Multiplication(x) with saturation for format Q31(32 bit)
 * 
 * @param     a     multiplier in format Q31(32bit)
 * @param     b     multiplicand in format Q31(32bit)
 * 
 * @return          product in format Q31(32bit)
 ******************************************************************************/
fxd_q31_t   fxd_mul(fxd_q31_t a, fxd_q31_t b);

/*******************************************************************************
 *  Multiply–accumulate operation with saturation for format Q63(64 bit)
 * 
 * @param     acum  accumulator in format Q63(64bit)
 * @param     b     multiplier in format Q31(32bit)
 * @param     c     multiplicand in format Q31(32bit)
 * 
 * @return          accumulator in format Q63(64bit)
 ******************************************************************************/
fxd_q63_t   fxd_mac(fxd_q63_t acum, fxd_q31_t b,  fxd_q31_t c);

/*******************************************************************************
 *  Multiply–accumulate(Subtraction) with saturation for format Q63(64 bit)
 * 
 * @param     a     accumulator in format Q63(64bit)
 * @param     b     multiplier in format Q31(32bit)
 * @param     c     multiplicand in format Q31(32bit)
 * 
 * @return          accumulator in format Q63(64bit)
 ******************************************************************************/
fxd_q63_t   fxd_msub(fxd_q63_t a, fxd_q31_t b,  fxd_q31_t c);

/*******************************************************************************
 *  Convert to absolute value  
 * 
 * @param     a     value in format Q31(32bit)
 * 
 * @return          absolute value in format Q31(32bit)
 ******************************************************************************/
fxd_q31_t   fxd_abs(fxd_q31_t a);

/*******************************************************************************
 *  Convert to negative 
 * 
 * @param     a     value in format Q31(32bit)
 * 
 * @return          negative value in format Q31(32bit)
 ******************************************************************************/
fxd_q31_t   fxd_neg(fxd_q31_t a);

/*******************************************************************************
 *  Left arithmetic signed shift(<<)
 * 
 * @param     a     value in format Q31(32bit)
 * @param     n     number of bits to shift(max = 32)
 * 
 * @return          shifted value in format Q31(32bit)
 ******************************************************************************/
fxd_q31_t   fxd_lshift(fxd_q31_t a , uint32_t n);

/*******************************************************************************
 *  Right arithmetic signed shift(>>)
 * 
 * @param     a     value in format Q31(32bit)
 * @param     n     number of bits to shift(max = 32)
 * 
 * @return          shifted value in format Q31(32bit)
 ******************************************************************************/
fxd_q31_t   fxd_rshift(fxd_q31_t a , uint32_t n);

/*******************************************************************************
 *  Convert float point value to fixed Q31 point value value 
 * 
 * @param     a     float value
 * 
 * @return          fixed value in format Q31(32bit)
 ******************************************************************************/
fxd_q31_t   flt_to_fxd(my_float input);

/*******************************************************************************
 *  Convert fixed point value Q31 to float point value
 * 
 * @param     a     fixed value in format Q31(32bit)
 * 
 * @return          float value in format float(32bit)
 ******************************************************************************/
my_float    fxd_to_flt(fxd_q31_t val);

/*******************************************************************************
 *  Convert float point value (64bit) to fixed point Q31 value 
 * 
 * @param     a     float value in format double(64bit)
 * 
 * @return          fixed value in format Q31(32bit)
 ******************************************************************************/
fxd_q31_t   dbl_to_fxd(double a);

/*******************************************************************************
 *  Convert fixed point value Q31 to float point (64bit) value 
 * 
 * @param     a     fixed value in format Q31(32bit)
 * 
 * @return          float value in format double(64bit)
 ******************************************************************************/
double      fxd_to_dbl(fxd_q31_t val);

/*******************************************************************************
 *  Division(÷) with saturation for format Q31(32 bit)
 * 
 * @param     N     devidend in format Q31(32bit)
 * @param     D     divisor in format Q31(32bit) (D!=0)
 * 
 * @return          quotient in format Q31(32bit)
 ******************************************************************************/
fxd_q31_t  fxd_div(fxd_q31_t N, fxd_q31_t D);

/*******************************************************************************
 *  Logarithm(log) with base 2
 * 
 * @param     n     anti-logarithm in format Q31(32bit)(n > 0)
 *
 * @return          logarithm in format Q5.26(32bit)
 ******************************************************************************/
fxd_q5_26_t   fxd_log2(fxd_q31_t n);

/*******************************************************************************
 *   Power(pow) with base 2
 * 
 * @param     n     power(exponent) in format Q5.26(32bit)(n < 0)
 *
 * @return          power in format Q31(32bit)
 ******************************************************************************/
fxd_q31_t   fxd_pow2(fxd_q5_26_t  n);

/*******************************************************************************
 *   Power(pow)
 * 
 * @param     a     base in format Q31(32bit)(a > 0)
 * @param     b     power(exponent) in format Q31(32bit)(b > 0)
 *
 * @return          power in format Q5.26(32bit)
 ******************************************************************************/
fxd_q31_t   fxd_pow(fxd_q31_t a, fxd_q31_t b);

//________________Additional_func_________________

/*******************************************************************************
 *  Addition(+) with saturation for format Q63(64bit)
 * 
 * @param     a     addendum in format Q63(64bit)
 * @param     b     addendum in format Q63(64bit)
 * 
 * @return          sum in format Q63(64bit)
 ******************************************************************************/
fxd_q63_t   fxd63_add(int64_t a, int64_t b);

/*******************************************************************************
 *  Subtraction(-) with saturation for format Q63(64bit)
 * 
 * @param     a     minuend in format Q63(64bit)
 * @param     b     subtrahend in format Q63(64bit)
 * 
 * @return          difference in format Q63(64bit)
 ******************************************************************************/
fxd_q63_t   fxd63_sub(fxd_q63_t a, fxd_q63_t b);


double      fxd5_26_to_dbl(int32_t val);

fxd_q5_26_t   dbl_to_fxd5_26(double input);

//print 

void print_n(fxd_q31_t num);

void print_n63(int64_t num);

fxd_q5_26_t     fxd_log2_no_interp(fxd_q31_t n);

fxd_q31_t       fxd_pow2_no_interp(fxd_q5_26_t  n);


fxd_q31_t       fxd_pow2_h(fxd_q31_t n);

fxd_q31_t       fxd_log2_h(fxd_q31_t n);

#endif /* colors.h */
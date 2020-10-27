//flt_arithmetic.c

#include "fxd_arithmetic.h"

#include <math.h>
#define BITS_TYPE32         31

#define BASE_BITS           26
#define CONV1               (FRACTION_BITS - BASE_BITS)
#define CONV                (BASE_BITS + BASE_BITS - FRACTION_BITS)
#define ONE                 ((1ll<<BASE_BITS))
#define coef                ((1ll<<BASE_BITS))
#define Xn                  ((48ll<<BASE_BITS)/17)
#define Xm                  ((32ll<<BASE_BITS)/17)

#define SIGN_Q31            (-1<<31)
#define SIGN_Q63            (-1ll<<63)

#define INTERP_FACTOR_MASK  ((1 << 22) - 1)

/***********************************************/

fxd_q63_t saturation(fxd_q63_t num){
    if(num > INT32_MAX){
        printf("sat Max");
        return INT32_MAX;
    }
    if(num < INT32_MIN){
                printf("SaM min Max");
        return INT32_MIN;
    }
    return num;
}

/***********************************************/

fxd_q31_t   fxd_add(int32_t a, int32_t b){
    fxd_q63_t res = a;
    res += b;

    res = saturation(res);

    return (fxd_q31_t)res;
}

/***********************************************/

fxd_q31_t   fxd_sub(fxd_q31_t a, fxd_q31_t b){
    int64_t res = a;
    res -= b;

    res = saturation(res);

    return (fxd_q31_t)res;
}

/***********************************************/

static fxd_q31_t   get_higher(fxd_q63_t a){
    a += (1ull << (FRACTION_BITS - 1));
    if(a == (1ull << 62) + (1ull << (FRACTION_BITS - 1))){
        a = (1ull << 62) - 1;
    }
    a >>= FRACTION_BITS;
    return (fxd_q31_t)a;
}

fxd_q31_t   fxd_mul(fxd_q31_t a, fxd_q31_t b){
    fxd_q63_t acum = a;

    acum *= b;
    return get_higher(acum);
}

/***********************************************/

fxd_q63_t   fxd_mac(fxd_q63_t a, fxd_q31_t b,  fxd_q31_t c){
    fxd_q63_t acum = b;
    acum *= c;
    if(acum == (1ull << 62)){
        acum = (1ull << 62) - 1;
    }
    acum <<= 1;
    acum = fxd63_add(acum, a);
    return acum;
}

/***********************************************/

fxd_q63_t   fxd_msub(fxd_q63_t a, fxd_q31_t b,  fxd_q31_t c){
    fxd_q63_t acum = b;
    acum *= c;
    if(a == (1ull << 62)){
        a = (1ull << 62) - 1;
    }
    acum <<= 1;
    acum = fxd63_sub(a, acum); 
    return acum;
}

/***********************************************/

fxd_q31_t   fxd_abs(fxd_q31_t a){
    if (a == FRACTIONAL_MIN) {
        return FRACTIONAL_MAX;
    }
    else {
        return (a < 0) ? -a: a;
    }
}

/***********************************************/
fxd_q63_t   fxd_mac_m(fxd_q63_t a, fxd_q31_t b,  fxd_q31_t c){
    fxd_q63_t acum = b;
    acum *= c;
    // if(acum == (1ull << 62)){
    //     acum = (1ull << 62) - 1;
    // }
    // acum <<= 1;
    acum = fxd63_add(acum, a);
    return acum;
}

/***********************************************/

fxd_q63_t   fxd_msub_m(fxd_q63_t a, fxd_q31_t b,  fxd_q31_t c){
    fxd_q63_t acum = b;
    acum *= c;

    // if(a == (1ull << 62)){
    //     a = (1ull << 62) - 1;
    // }
    // acum <<= 1;
    acum = fxd63_sub(a, acum); 
    return acum;
}
/***********************************************/

fxd_q31_t   fxd_neg(fxd_q31_t a){

    return (a == FRACTIONAL_MIN) ? FRACTIONAL_MAX : -a;
}

/***********************************************/

fxd_q31_t   fxd_rshift(fxd_q31_t a , uint32_t n){
    assert(n <= 32);
    
    fxd_q31_t  res =  a >> n;

    return res;
}

/***********************************************/

fxd_q31_t   fxd_lshift(fxd_q31_t a, uint32_t n){
    
    assert(n <= 32);

    fxd_q31_t  res =  a << n;

    if((a ^ res) & SIGN_Q31){
        res = (a > 0) ? (res ^ SIGN_Q31) : (res | SIGN_Q31);
    }
    return res;
}

/***********************************************/

fxd_q31_t   flt_to_fxd(my_float inp)
{
    if(inp >= 1.0){
        return FRACTIONAL_MAX;
    }
    if(inp <= -1.0){
        return FRACTIONAL_MIN;
    }
    return (fxd_q31_t)(inp * (FRACTIONAL_BASE));
}

/***********************************************/

my_float    fxd_to_flt(fxd_q31_t val){
    return ((float)val / (float)(1u << FRACTION_BITS));
}

/***********************************************/

fxd_q31_t   dbl_to_fxd(double a)
{
    if(a >= 1.0){
        return FRACTIONAL_MAX;
    }
    if(a <= -1.0){
        return FRACTIONAL_MIN;
    }
    return (fxd_q31_t)(a * (FRACTIONAL_BASE));
}

/***********************************************/

double      fxd_to_dbl(fxd_q31_t a){
    return ((double)a / (double)(1u << FRACTION_BITS));
}
/***********************************************/

static int64_t div_mul(int64_t a, int64_t b){
    return ((a * b) >> BASE_BITS);
}

fxd_q31_t  fxd_div(fxd_q31_t N, fxd_q31_t D){
    assert(D != 0);
    int64_t  Dsh = fxd_abs(D) >> CONV1;
    int64_t  Nsh = fxd_abs(N) >> CONV1;
    int64_t x = Xn - div_mul(Xm, Dsh);

    for(int i = 0; i < 3; i++){
        x = x + div_mul(x, (ONE - div_mul(Dsh, x)));
    }
    x = (Nsh * x) >> CONV;
    x = saturation(x);
    if((N ^ D) & SIGN_Q31){
        x = fxd_neg((fxd_q31_t)x);
    }
    return  (fxd_q31_t)x;
}

/***********************************************/

fxd_q31_t   fxd_pow2(fxd_q5_26_t  n){

    assert(n < 0); 

    int32_t index = -(n>>22);
    int32_t interp_factor = (n & INTERP_FACTOR_MASK_LOG) ;
    int64_t tmp = 0;

    tmp = POW2_FXD_Q26[index - 1] - POW2_FXD_Q26[index];
    tmp = tmp - ((tmp * interp_factor) >> 22);
    tmp =  POW2_FXD_Q26[index] - tmp;
    return (fxd_q31_t )tmp;
}

/***********************************************/

fxd_q5_26_t   fxd_log2(fxd_q31_t n){
    assert(n > 0);

    int32_t index = (n>>22);
    int32_t interp_factor = n & INTERP_FACTOR_MASK;
    int64_t tmp = 0;

    tmp = LOG2_FXD_Q27[index + 1] - LOG2_FXD_Q27[index];
    tmp = (tmp * interp_factor) >> 22;
    tmp = LOG2_FXD_Q27[index] + tmp;
    return (fxd_q5_26_t)tmp;
}

/***********************************************/

fxd_q31_t   fxd_pow(fxd_q31_t a, fxd_q31_t b){

    assert(a > 0 && b > 0 );

    int64_t tmp = fxd_log2(a);
    tmp *= b;
    tmp >>= 31;
    tmp = saturation(tmp);
    return fxd_pow2((fxd_q5_26_t)tmp);
}

/***********************************************/

//________________Additional_func_________________

/***********************************************/

fxd_q63_t   fxd63_add(int64_t a, int64_t b){
    fxd_q63_t res;

    res = a + b;

    if (!((a ^ b) & SIGN_Q63))
    {    

        if ((res ^ a) & SIGN_Q63)
        {
            res = (a < 0) ? (-1ll<<63) + 1: ((1ull<<63)-1);
        }
    }
    return res;
}

/***********************************************/

fxd_q63_t   fxd63_sub(fxd_q63_t a, fxd_q63_t b){
    fxd_q63_t res = 0;

    res = a - b;
    
    if (((a ^ b) & SIGN_Q63))
    {
        if ((res ^ a) & SIGN_Q63) 
        {
            res = (a < 0) ? (-1ll<<63) : ((1ull<<63)-1);
        }
    }

    return res;
}

/***********************************************/

fxd_q63_t   fxd63_lshift(fxd_q63_t a, uint32_t n){
    fxd_q63_t res ;

    assert(n <= 64);
    res =  a << n;

    if((a ^ res) & SIGN_Q63){
        res = (a > 0) ? res ^ (-1ll<<63) : res | (-1ll<<63);
    }
    return res;
}

/***********************************************/

fxd_q63_t   fxd63_rshift(fxd_q63_t a, uint32_t n){
    fxd_q63_t res ;

    assert(n <= 64);
    res =  a >> n;

    return res;
}

/***********************************************/

fxd_q31_t   fxd_pow2_h(fxd_q31_t n){

    assert(n <= -(1<<22));

    int32_t index = -(n>>22) - 1;
    int32_t interp_factor = n & INTERP_FACTOR_MASK;
    int64_t tmp = 0;
    
    tmp = POW2_FXD[index - 1] - POW2_FXD[index];
    tmp = (tmp * interp_factor) >> 22;
    tmp =  POW2_FXD[index] + tmp;
    return (fxd_q31_t)tmp;
}

/***********************************************/

fxd_q31_t   fxd_log_h(fxd_q31_t n){

    assert(n >= HALF_Q31_PL && n <= ((511u<<21) + HALF_Q31_PL ));
    n -= (HALF_Q31_PL);

    int32_t index = (n>>21);
    int32_t interp_factor = n & INTERP_FACTOR_MASK_LOG;
    int64_t tmp = 0;

    tmp = LOG2_FXD[index] - LOG2_FXD[index + 1];
    tmp = (tmp * interp_factor) >> 21;
    tmp =  LOG2_FXD[index] - tmp;
    return (fxd_q31_t)tmp;
}

/***********************************************/

fxd_q5_26_t   fxd_log2_no_interp(fxd_q31_t n){

    assert(n > 0);

    int32_t index = (n>>22);
    int32_t interp_factor = n & INTERP_FACTOR_MASK;
    int64_t tmp = 0;

    tmp = LOG2_FXD_Q27[index] + tmp;
    return (fxd_q5_26_t)tmp;
}

/***********************************************/

fxd_q31_t   fxd_pow2_no_interp(fxd_q5_26_t  n){
    assert(n < 0); 
    int32_t index = -(n>>22);
    int32_t interp_factor = (-n & INTERP_FACTOR_MASK_LOG) ;
    int64_t tmp = 0;

    tmp =  POW2_FXD_Q26[index];

    return (fxd_q31_t )tmp;
}


/***********************************************/

static void binaryPrint(uint32_t num){
    uint32_t b = (1ul<<BITS_TYPE32);
                // printf("\n");
    for(int i = 0; i <= BITS_TYPE32; i++){
        if ((num & b) == b){
            printf("1");
        }else{
            printf("0");
        }
        b >>= 1;
    }
    printf("\n");
}

void print_n(fxd_q31_t num){
    uint32_t tmp;
    memcpy(&tmp, &num, 4);
    binaryPrint(tmp);
}

/***********************************************/

static void binaryPrint63(uint64_t num){
    uint64_t b = (1ull<<63);
                // printf("\n");
    for(int i = 0; i <= 63; i++){
        if ((num & b) == b){
            printf("1");
        }else{
            printf("0");
        }
        b >>= 1;
    }
    printf("\n");
}

void print_n63(int64_t num){
    uint64_t tmp ;
    memcpy(&tmp, &num, 8);
    binaryPrint63((uint64_t)num);
}

/***********************************************/

double fxd_div_to_dbl(int64_t val){

    return ((double)val / (double)(1u << BASE_BITS));
}

/***********************************************/

double fxd31_to_dbl(int64_t val){
    return ((double)val / (double)(1u << 31));
}

/***********************************************/

double fxd5_26_to_dbl(int32_t val){
    return ((double)val / (double)(1u << 26));
}

/***********************************************/

fxd_q5_26_t   dbl_to_fxd5_26(double input)
{
    return (fxd_q5_26_t)(input * (1u << 26));
}

/***********************************************/

double      fxd64_to_flt(fxd_q63_t val){
    return ((double)val / (double)(1u << FRACTION_BITS));
}

/***********************************************/
fxd_q63_t   fxd_fmul(fxd_q31_t a, fxd_q31_t b){
    fxd_q63_t acum = a;
    // print_n63(acum);
    acum *= b;
    // print_n63(acum);
    return acum;
}


fxd_q31_t   dbl_to_fxd_p(double a, uint32_t p)
{
    assert(p <= 31);
    return (fxd_q31_t)(a * (1u<<p));
}

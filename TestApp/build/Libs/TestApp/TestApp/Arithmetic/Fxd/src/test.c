#include <fxd_arithmetic.h>
#include <math.h>
#include <inttypes.h>
#include "flt_arithmetic.h"

// #define M_E         2.71828182845904523536028747135266250


int sat(uint8_t a, uint8_t b){
    int8_t re = INT8_MAX - b;
    printf("re = %d\n", re);
    if(a < re){
        printf("sat YES\n");
    }else
    {
        printf("sat NO = %d\n",a);
    }
    return 0;
}

static int32_t test_div();
static int32_t test_log2();
static int32_t test_pow2();
static int32_t test_pow();
static int32_t test_log2_h();
static int32_t test_pow2_h();

int main(){
    // int32_t a = 0b00000000000000001000010000000000;
    // int32_t b = 0b00000000000000001000100000000000;

    // int32_t a = 0b00000000000000001000010000000000;
    // int32_t b = 0b00000000000000001000100000000000;

    // int32_t a = 0b01111111111111110000000000000000;
    // int32_t b = 0b01111111111111110000000000000000;

    // int32_t a = 0b11111111111111110000000000000001;
    // int32_t b = 0b11111111111111110000000000000000;

    // int32_t a = 0b01111111111111111111111111111111;
    // int32_t b = 0b01111111111111111111111111111111;

    // int32_t a = 0b11111111111111111111111111111111;
    // int32_t b = 0b11111111111111111111111111111111;

    int32_t a = 0b01111111111111111111111111111111;

    int32_t d =  0b10000000000000000000000000000000;

    // int64_t i = fxd_mac(0, INT32_MIN, INT32_MIN);
    // printf("int %lli %d\n", i, INT32_MAX);
    test_pow2();
    // fxd_pow2(-1);
    // test_log2();
    // for(int i = 512; i >= 0; i--){
    //     printf( "%d, \n", POW2_FXD_Q26[i]);
    // }
        
}
static int32_t test_div(){
    int32_t res_q31 = 0;
    double i = -1.0;
    for(int p = 0; p < 10; p++){
        // if(i == 0.0){
        //     printf("im hehre\n");
        //     continue;
        // }
        res_q31 = fxd_div(HALF_Q31_PL,  dbl_to_fxd(i));
        flt_div(0.5f, (float)i);
        // break;
        printf("0.5/%f  %5.10f  %5.10f myres %5.10f\n", i, -0.5/i, flt_div(-0.5f, (float)i), fxd_to_dbl(res_q31) );
         i = i + 0.1;
    }

    return 0;
}

static int32_t test_pow(){
    
    int32_t a_fxd = INT32_MAX;

    double a = 0.5;//fxd_to_dbl(a_fxd);
    double res = 0;
    int32_t res_fxd = 0;
    double n = 0;
    double average = 0;
    for (a = 0.1 ; a < 1; a = a + 0.1){
        for (double i = 0.1; i < 1; i = i + 0.1){
        // res_fxd = fxd_pow(a_fxd, dbl_to_fxd(i));
            res = pow(a, i);
            res_fxd  = fxd_pow( dbl_to_fxd(a), dbl_to_fxd(i)); //
            printf("pow(%5.10f ,%5.10f ) = %5.10f my =  %5.10f\n", a ,i, res,  fxd_to_dbl(res_fxd));
            average = average + fabs(pow(a, i)) - fabs(fxd_to_dbl(res_fxd));
            n = n + 1;
        }
    }
    printf("avarange = %f\n", average/n);

    return 0;
}

static double abs_diff(double n, double d){
    return fabs(fabs(n) - fabs(d));
}

static int32_t test_log2(){
    int32_t fxd_p = 0;
    double tmp = 0;
    int32_t tmp_log = 0;
    
    for (int32_t i = 0; i <= 511; i++){
        
        fxd_p = (i<<22);
        if(i == 0){
            fxd_p = 1;
        }
        tmp_log = fxd_log2(fxd_p);
        tmp = log2(fxd_to_dbl(fxd_p));
        printf("%d,// log2(%5.10f) = %5.10f my  %5.10f\n",  dbl_to_fxd5_26(tmp), fxd_to_dbl(fxd_p),  log2(fxd_to_dbl(fxd_p)),   fxd5_26_to_dbl(tmp_log));
    }
    printf("\n+++++++++++++++++++++++MAX+++++++++++++++++++\n");
    fxd_p = INT32_MAX;
    tmp_log = fxd_log2(fxd_p);
    tmp = log2(fxd_to_dbl(fxd_p));
    printf("%d,// log2(%5.10f) = %5.10f my  %5.10f\n",  dbl_to_fxd5_26(tmp), fxd_to_dbl(fxd_p),  log2(fxd_to_dbl(fxd_p)),   fxd5_26_to_dbl(tmp_log));

    printf("\n+++++++++++++++++++++++check interp+++++++++++++++++++\n");
    fxd_p = (1<<22) - 1;
    tmp_log = fxd_log2(fxd_p);
    tmp = log2(fxd_to_dbl(fxd_p));
    printf("%d,// log2(%5.10f) = %5.10f my  %5.10f\n",  dbl_to_fxd5_26(tmp), fxd_to_dbl(fxd_p),  log2(fxd_to_dbl(fxd_p)),   fxd5_26_to_dbl(tmp_log));
    for(int i = 1; i  <= (1<<22); i <<= 1){
        fxd_p = (1<<22) + i - 1;
        tmp_log = fxd_log2(fxd_p);
        tmp = log2(fxd_to_dbl(fxd_p));
        printf("%d,// log2(%5.10f) = %5.10f my  %5.10f\n",  dbl_to_fxd5_26(tmp), fxd_to_dbl(fxd_p),  log2(fxd_to_dbl(fxd_p)),   fxd5_26_to_dbl(tmp_log));
    }

    double delta = 0;
    double delta_no_interp = 0;
    double real_val = 0;
    double real_val_no_interp = 0;
    double average_no_interp = 0;
    double average = 0;

    for (int32_t i = 1; i < (INT32_MAX/4); i++){
        double tmp =  abs_diff(log2(fxd_to_dbl(i)), fxd5_26_to_dbl(fxd_log2(i)));
        if(tmp > delta){
            delta = tmp;
            real_val = log2(fxd_to_dbl(i));
            // printf("%f %f %f\n", fxd5_26_to_dbl(fxd_log2(i)), log2(fxd_to_dbl(i)),  fxd_to_dbl(i));
        }
        average += tmp ;
        tmp = abs_diff(log2(fxd_to_dbl(i)), fxd5_26_to_dbl(fxd_log2_no_interp(i)));
        if(tmp > delta_no_interp){
            //  printf(" delta_no_interp %f\n",  delta_no_interp);
            delta_no_interp = tmp;
            real_val_no_interp = log2(fxd_to_dbl(i));
        }
        average_no_interp += tmp ;
    }
    printf(" delta_no_interp %f\n",  delta_no_interp);
    average_no_interp = average_no_interp / (INT32_MAX/16);
    average = average / (INT32_MAX/4);
    printf("\nModule  DELTA interp %5.10f %5.10f average = %f\n",delta, (delta/real_val*100),  average);
    printf("\nModule  DELTA  no interp %5.10f %5.10f average = %f\n",delta_no_interp, (delta_no_interp/real_val_no_interp*100), average_no_interp);
    return 0;
}

static int32_t test_pow2(){
 int32_t fxd_p = 0;
    double tmp = 0;
    int32_t tmp_pow  = 0; 
    printf("\n+++++++++++++++++++++++CHECK POW+++++++++++++++++++\n");
    for (int32_t i = 1; i < 10; i++){

        fxd_p = (-i << 22);
        // fxd_p  = -1;
        tmp = pow(2.0, fxd5_26_to_dbl(fxd_p));

        printf("%d,// pow2(%5.10f) = %5.10f  , %d %5.10f\n",  dbl_to_fxd(tmp), fxd5_26_to_dbl(fxd_p), tmp , i, fxd_to_dbl(fxd_pow2(fxd_p)));  /*, fxd_log2(dbl_to_fix(it))*/
    }
     printf("\n+++++++++++++++++++++++CHECK INTERP+++++++++++++++++++\n");
        fxd_p = (-1<<22);
        tmp_pow = fxd_pow2(fxd_p);
        tmp = pow(2.0, fxd5_26_to_dbl(fxd_p));

        printf("%d,// pow2(%5.10f) = %5.10f  , %d %5.10f\n",  dbl_to_fxd(tmp), fxd5_26_to_dbl(fxd_p), tmp , 0, fxd_to_dbl(fxd_pow2(fxd_p)));  /*, fxd_log2(dbl_to_fix(it))*/
    

    for(int i = -1; i  >= (-1<<22); i <<= 1){
        fxd_p = i  + (-1<<22);
        tmp_pow = fxd_pow2(fxd_p);
        tmp = pow(2.0, fxd5_26_to_dbl(fxd_p));

        printf("%d,// pow2(%5.10f) = %5.10f  , %d %5.10f\n",  dbl_to_fxd(tmp), fxd5_26_to_dbl(fxd_p), tmp , i, fxd_to_dbl(fxd_pow2(fxd_p)));  /*, fxd_log2(dbl_to_fix(it))*/
    }

    double delta = 0;
    double delta_no_interp = 0;
    double real_val = 0;
    double real_val_no_interp = 0;
    double average_no_interp = 0;
    double average = 0;

    for (int32_t i = -1; i > (INT32_MIN/4); i--){
        double tmp = fabs(pow(2.0, fxd5_26_to_dbl(i))) -  fabs(fxd_to_dbl(fxd_pow2(i)));
        if(tmp > delta){
            delta = tmp;
            real_val = (pow(2.0, fxd5_26_to_dbl(i)));
        }
        average += tmp ;
        tmp = fabs(pow(2.0, fxd5_26_to_dbl(i))) -  fabs(fxd_to_dbl(fxd_pow2_no_interp(i)));
        if(tmp > delta_no_interp){
            delta_no_interp = tmp;
            real_val_no_interp =(pow(2.0, fxd5_26_to_dbl(i)));
        }
        average_no_interp += tmp ;
    }
    average_no_interp = average_no_interp / (-INT32_MIN/4);
    average = average / (-INT32_MIN/4);
    printf("\nModule  DELTA interp %5.10f %5.10f average = %f\n",delta, (delta/real_val*100),  average);
    printf("\nModule  DELTA  no interp %5.10f %5.10f average = %f\n",delta_no_interp, (delta_no_interp/real_val_no_interp*100), average_no_interp);
    return 0;
}

static int32_t test_log2_h(){
    int32_t fxd_p = 0;
    double tmp = 0;
    int32_t tmp_log = 0;
    
    for (uint32_t i = 1; i < 512; i++){
        fxd_p = (i << 22);
        tmp = log2(fxd_to_dbl(fxd_p));
        printf("%d,// log2(%5.10f) = %5.10f", dbl_to_fxd(tmp),fxd_to_dbl(fxd_p), log2(fxd_to_dbl(fxd_p)));
       // printf("%d,// log2(%5.10f) = %5.10f  , %d %5.10f \n", dbl_to_fxd(tmp), fxd_to_dbl(fxd_p), tmp , i, fxd_to_dbl(fxd_log2(fxd_p)));  /*, fxd_log2(dbl_to_fix(it))*/
    }

    // for (uint32_t i = 0; i < 512; i++){
    //     fxd_p =  (i<<21) + HALF_Q31_PL;
    //     tmp = log2(fxd_to_dbl(fxd_p));

    //     printf("%d,// log2(%5.10f) = %5.10f  , %d %5.10f \n", dbl_to_fxd(tmp), fxd_to_dbl(fxd_p), tmp , i, fxd_to_dbl(fxd_log2(fxd_p)));  /*, fxd_log2(dbl_to_fix(it))*/

    // }

    // printf("\n+++++++++++++++++++++++check interp+++++++++++++++++++\n");
    // fxd_p = HALF_Q31_PL;
    // tmp = log2( fxd_to_dbl(fxd_p));
    // tmp_log = fxd_log2(fxd_p);
    // printf("%d,// pow2(%5.10f) = %5.10f  %5.10f\n", tmp_log, fxd_to_dbl(fxd_p), tmp , fxd_to_dbl(tmp_log));  /*, fxd_log2(dbl_to_fix(it))*/

    // for(int i = 1; i  <= (1<<21); i <<= 1){
    //     fxd_p = HALF_Q31_PL + i;
    //     tmp = log2(fxd_to_dbl(fxd_p));
    //     tmp_log = fxd_log2(fxd_p);
    //     printf("%d,// pow2(%5.10f) = %5.10f  %5.10f\n", tmp_log, fxd_to_dbl(fxd_p), tmp , fxd_to_dbl(tmp_log));  /*, fxd_log2(dbl_to_fix(it))*/
    // }

    // fxd_p =  (0<<21) + HALF_Q31_PL + 1;
    // tmp = log2(fxd_to_dbl(fxd_p));
    // printf("%d,// log2(%5.10f) = %5.10f  %5.10f\n", fxd_log2(fxd_p), fxd_to_dbl(fxd_p), tmp , fxd_to_dbl(fxd_log2(fxd_p)));  /*, fxd_log2(dbl_to_fix(it))*/
    
    // fxd_p =  (1<<21) + HALF_Q31_PL + 1;
    // tmp = log2(fxd_to_dbl(fxd_p));
    // printf("%d,// log2(%5.10f) = %5.10f  %5.10f\n", fxd_log2(fxd_p), fxd_to_dbl(fxd_p), tmp , fxd_to_dbl(fxd_log2(fxd_p)));  /*, fxd_log2(dbl_to_fix(it))*/
    
    // fxd_p =  (0<<21) + HALF_Q31_PL + 1 + 500;
    // tmp = log2(fxd_to_dbl(fxd_p));
    // printf("%d,// log2(%5.10f) = %5.10f  %5.10f\n", fxd_log2(fxd_p), fxd_to_dbl(fxd_p), tmp , fxd_to_dbl(fxd_log2(fxd_p)));  /*, fxd_log2(dbl_to_fix(it))*/
    
    // fxd_p =  (0<<21) + HALF_Q31_PL + 1 + 15000;
    // tmp = log2(fxd_to_dbl(fxd_p));
    // printf("%d,// log2(%5.10f) = %5.10f  %5.10f\n", fxd_log2(fxd_p), fxd_to_dbl(fxd_p), tmp , fxd_to_dbl(fxd_log2(fxd_p)));  /*, fxd_log2(dbl_to_fix(it))*/
    
    return 0;
}

static int32_t test_pow2_h(){
    int32_t fxd_p = 0;
    double tmp = 0;
    int32_t tmp_pow  = 0; 
    printf("\n+++++++++++++++++++++++CHECK POW+++++++++++++++++++\n");
    for (int32_t i = 512; i > 0; i--){

        fxd_p = (-i << 22);

        tmp = pow(2.0, fxd_to_dbl(fxd_p));

        printf("%d,// pow2(%5.10f) = %5.10f  , %d %5.10f\n",dbl_to_fxd(tmp), fxd_to_dbl(fxd_p), tmp , i, fxd_to_dbl(fxd_pow2(fxd_p)));  /*, fxd_log2(dbl_to_fix(it))*/
        //fxd_to_dbl(fxd_pow2(fxd_p));
    }
    printf("\n+++++++++++++++++++++++check interp+++++++++++++++++++\n");
    fxd_p = INT32_MIN ;
    tmp = pow(2.0, fxd_to_dbl(fxd_p));
    tmp_pow = fxd_pow2(fxd_p);
    printf("%d,// pow2(%5.10f) = %5.10f  %5.10f\n", tmp_pow, fxd_to_dbl(fxd_p), tmp , fxd_to_dbl(tmp_pow));  /*, fxd_log2(dbl_to_fix(it))*/
    for(int i = 1; i  <= (1<<22); i <<= 1){
        fxd_p = INT32_MIN + i;
        tmp = pow(2.0, fxd_to_dbl(fxd_p));
        tmp_pow = fxd_pow2(fxd_p);
        printf("%d,// pow2(%5.10f) = %5.10f  %5.10f\n", tmp_pow, fxd_to_dbl(fxd_p), tmp , fxd_to_dbl(tmp_pow));  /*, fxd_log2(dbl_to_fix(it))*/
    }
    return 0;

}


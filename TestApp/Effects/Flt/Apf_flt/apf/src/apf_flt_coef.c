#include "apf_flt.h"

int32_t apf_flt_1st_coef(apf_coef_t *coef, double fc, double sample_rate, int32_t form){
    double c = (tan(M_PI *  fc / sample_rate) - 1) / (tan(M_PI *  fc / sample_rate) + 1);

    coef->k[0] = (coef_type)c;
    printf("First freq %f\n",  fc);
    printf("coef->k[0] = %f\n", coef->k[0]);
    

    return 0;
}

int32_t apf_flt_2nd_coef(apf_coef_t *coef, double fc, double sample_rate, int32_t form){
    double Q = 0.1;
    double fb = 0.8;

    double c = (tan(M_PI *  fb / sample_rate) - 1) / (tan(M_PI *  fb / sample_rate) + 1);
    double d = -cos(2 * M_PI  * fc / sample_rate);
    double dc = d * (1.0 - c);

    coef->k[0] = (coef_type)dc;
    coef->k[1] = (coef_type)-c;
        printf("SEcond freq%f\n",  fc);
    printf("coef->k[0] = %f\n", coef->k[0]);
    printf("coef->k[1] = %f\n", coef->k[1]);

    coef->k_dbl[0] = dc;
    coef->k_dbl[1] = -c;
    if(form == 5){//Nested form coef
        coef->k[0] = (coef_type)d;
        coef->k[1] = (coef_type)-c;
        coef->k_dbl[0] = d;
        coef->k_dbl[1] = -c;
    }
    return 0;
}

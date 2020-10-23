#include "apf_flt.h"

static int32_t convet_coefs(apf_doub_coefs_t *_coef, apf_coefs_t *coef);

static double dB_to_coeff(double dB){
    return pow(10.0f , (0.05f * dB));
}

uint32_t fact(uint32_t n) 
{ 
    if (n == 0) 
        return 1; 
    return n * fact(n - 1); 
} 

int32_t apf_coeff_calc(apf_prm_t *_prm, apf_coefs_t *coef){
    apf_doub_coefs_t _coef;
    double N = (double)APF_ORDER;
    double c = 0.0;

    double D = tan(M_PI * _prm->freq / _prm->sample_rate);
    printf("tan(M_PI * _prm->freq / _prm->sample_rate)= %f\n", tan(M_PI * _prm->freq / _prm->sample_rate));
    for(uint32_t k = 1; k <= APF_ORDER; k++){                     // for k = 0,1,2,.., N coeff index
        _coef.c[k - 1] = pow(-1.0, k);
            printf("c = %f   0 _coef.c[k - 1] %f\n", c,  _coef.c[k - 1]);
        _coef.c[k - 1] *= (double)fact(APF_ORDER)/(double)(fact(k) * fact(APF_ORDER - k));
        printf("c = %f   1 _coef.c[k - 1] %f\n", c,  _coef.c[k - 1]);
        for(uint32_t n = 0; n <= APF_ORDER; n++){
            _coef.c[k - 1] *= (D - N + (double)n)/(D - N + (double)n + (double)k);
             printf("c = %f  _coef.c[k - 1] %f %f\n", c,  _coef.c[k - 1], (double)k);
        }
    }
    c = (D - 1.0)/(D + 1.0);
    printf("c = %f _coef.c[k] %f\n", c, _coef.c[0]);
    convet_coefs(&_coef, coef);
        c = (D - 1.0)/(D + 1.0);
    return 0;
}

static int32_t convet_coefs(apf_doub_coefs_t *_coef, apf_coefs_t *coef){
    for(uint32_t k = 0; k < APF_ORDER; k++){
        coef->c[k] = (coef_type)_coef->c[k];
    }
    return 0;
}

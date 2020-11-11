#include "cross_flt.h"

static double dB_to_coeff(double dB){
    return pow(10.0f , (0.05f * dB));
}
static int32_t apf_flt_1st_coef(apf_coef_1st_t *coef, double fc, double sample_rate);
static int32_t apf_flt_2nd_coef(apf_coef_2nd_t *coef, double fc, double sample_rate);

int32_t cross_flt_coef(cross_prm_t *prm, cross_flt_coef_t *coef){
    for(int freq = 0; freq < NUM_OF_FREQ; freq++){
        apf_flt_1st_coef(&(coef->apf_1st[freq]), prm->freq[freq], prm->sample_rate);
        apf_flt_2nd_coef(&(coef->apf_2nd[freq]), prm->freq[freq], prm->sample_rate);
    }
    return 0;
}

static int32_t apf_flt_1st_coef(apf_coef_1st_t *coef, double fc, double sample_rate){
    double c = (tan(M_PI *  fc / sample_rate) - 1) / (tan(M_PI *  fc / sample_rate) + 1);

    coef->k1 = (coef_type)c;

    return 0;
}

static int32_t apf_flt_2nd_coef(apf_coef_2nd_t *coef, double fc, double sample_rate){
    double Q = 0.1;
    double fb = 0.8;

    double c = (tan(M_PI *  fb / sample_rate) - 1) / (tan(M_PI *  fb / sample_rate) + 1);
    double d = -cos(2 * M_PI  * fc / sample_rate);
    double dc = d * (1.0 - c);

    coef->k1 = (coef_type)dc;
    coef->k2 = (coef_type)-c;

    return 0;
}

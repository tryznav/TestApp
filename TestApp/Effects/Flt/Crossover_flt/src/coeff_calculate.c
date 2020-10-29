#include "cross_flt.h"

static double dB_to_coeff(double dB){
    return pow(10.0f , (0.05f * dB));
}

int32_t cross_coeff_calc(cross_prm_t *prm, coef_t *coef){
    for(int freq = 0; freq < NUM_OF_FREQ; freq++){
        double fb = 0.8;// (sample_rate / Q);

        double c = (tan(M_PI *  fb / prm->sample_rate) - 1) / (tan(M_PI *  fb / prm->sample_rate) + 1);
        double d = - cos(2 * M_PI  * prm->freq[freq] / prm->sample_rate);

        coef->apf_2nd[freq].k[0]  = (coef_type)d;
        coef->apf_2nd[freq].k[1]  = (coef_type)-c;

        c = (tan(M_PI *  prm->freq[freq] / prm->sample_rate) - 1) / (tan(M_PI *  prm->freq[freq] / prm->sample_rate) + 1);

        coef->apf_1st[freq].k[0]  = (coef_type)c;
        printf("c = %f", c);

    }

    for(int i = 0; i < NUM_OF_BAND; i++){
        coef->gain_c[i] = (coef_type)dB_to_coeff(prm->gain_dB[i]);
    }
    return 0;
}

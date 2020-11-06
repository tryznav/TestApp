#include "cross_fxd.h"

static double dB_to_coeff(double dB){
    return pow(10.0f , (0.05f * dB));
}

int32_t cross_coeff_calc(cross_prm_t *prm, coef_t *coef){
    for(int freq = 0; freq < NUM_OF_FREQ; freq++){
        double fb = 0.8;// (sample_rate / Q);

        double c = (tan(M_PI *  fb / prm->sample_rate) - 1) / (tan(M_PI *  fb / prm->sample_rate) + 1);
        double d = - cos(2 * M_PI  * prm->freq[freq] / prm->sample_rate);
        double dc = d * (1.0 - c);

        coef->apf_2nd[freq].k[0]  = dbl_to_fxd_p(d, COEF_FR);
        coef->apf_2nd[freq].k[1]  = dbl_to_fxd_p((-c), COEF_FR);

        coef->apf_2nd[freq].k[0] = dbl_to_fxd_p((dc/2.0), COEF_FR);
        coef->apf_2nd[freq].k[1] = dbl_to_fxd_p((-c/2.0), COEF_FR);

        c = (tan(M_PI *  prm->freq[freq] / prm->sample_rate) - 1) / (tan(M_PI * prm->freq[freq] / prm->sample_rate) + 1);

        coef->apf_1st[freq].k[0]  = dbl_to_fxd_p(c, COEF_FR);

    }

    for(int i = 0; i < NUM_OF_BAND; i++){
        coef->gain_c[i] = dbl_to_fxd_p (dB_to_coeff(prm->gain_dB[i]), COEF_FR);
    }
    return 0;
}

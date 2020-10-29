#include "apf_flt.h"

static int32_t apf_coeff_2nd_calc(apf_coefs_t *coef, double fc, double sample_rate);
static int32_t apf_coeff_1st_calc(apf_coefs_t *coef, double fc, double sample_rate);

int32_t apf_coeff_calc(apf_prm_t *prm, apf_coefs_t *coef){
    coef->order = prm->order;
    coef->form = prm->form;
    switch (prm->order)
    {
    case 1:
        apf_coeff_1st_calc(coef, prm->freq,  prm->sample_rate);
        break;
    case 2:
        apf_coeff_2nd_calc(coef, prm->freq,  prm->sample_rate);
        break;
    default:
        break;
    }
    return 0;
}

static int32_t apf_coeff_2nd_calc(apf_coefs_t *coef, double fc, double sample_rate){
    double Q = 0.1;//1.0 / pow(2.0, 0.5);
    double fb = (0.8);// (sample_rate / Q);
    printf("fb = %f\n", fb);
    double c = (tan(M_PI *  fb / sample_rate) - 1) / (tan(M_PI *  fb / sample_rate) + 1);
    double d = -cos(2 * M_PI  * fc / sample_rate);
    double dc = d * (1.0 - c);
    printf("c = %f\n", c);
    printf("dc = %f\n", dc);
    printf("d = %f\n", d);

    switch (coef->form)
    {
    case 1:     // direct form
        coef->c[0] = (coef_type)c;
        coef->c[1] = (coef_type)dc;
        coef->c_dbl[0] = c;
        coef->c_dbl[1] = dc;
        break;
    case 2:     // lattice form
        coef->c[0] = (coef_type)d;
        coef->c[1] = (coef_type)-c;
        coef->c_dbl[0] = d;
        coef->c_dbl[1] = -c;
        break;

    default:
        break;
    }

    return 0;
}

static int32_t apf_coeff_1st_calc(apf_coefs_t *coef, double fc, double sample_rate){
    double c = (tan(M_PI *  fc / sample_rate) - 1) / (tan(M_PI *  fc / sample_rate) + 1);
    // c = 2.0f;
    coef->c[0] = (coef_type)c;
    printf("c = %f\n", c);
    return 0;
}

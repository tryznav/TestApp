#include "apf_fxd.h"

static int32_t apf_coeff_2nd_calc(apf_coefs_t *coef, double fc, double sample_rate, int32_t form);
static int32_t apf_coeff_1st_calc(apf_coefs_t *coef, double fc, double sample_rate);

int32_t apf_coeff_calc_fxd(apf_prm_t *prm, apf_coefs_t *coef){
    coef->order = prm->order;
    coef->form = prm->form;
    switch (prm->order)
    {
    case 1:
        apf_coeff_1st_calc(coef, prm->freq,  prm->sample_rate);
        break;
    case 2:
        apf_coeff_2nd_calc(coef, prm->freq,  prm->sample_rate, prm->form);
        break;
    default:
        break;
    }
    return 0;
}

static int32_t apf_coeff_2nd_calc(apf_coefs_t *coef, double fc, double sample_rate, int32_t form){
    double Q = 1.0 / pow(2.0, 0.5);
    double fb = 0.8;// (sample_rate / Q);
    printf("fb = %f\n", fb);
    double c = (tan(M_PI *  fb / sample_rate) - 1) / (tan(M_PI *  fb / sample_rate) + 1);
    double d = - cos(2 * M_PI  * fc / sample_rate);
    double dc = d * (1.0 - c);

    // switch (form)
    // {
    // case 1:     // direct form

    //     break;
    // case 2:     // lattice form
    //     printf("c = %f\n", c);
    //     printf("d = %f\n", d);
    //     coef->c[0] = dbl_to_fxd_p(d, COEF_FR);
    //     coef->c[1] = dbl_to_fxd_p((-c), COEF_FR);
    //     coef->c_dbl[0] = d;
    //     coef->c_dbl[1] = -c;
    //     break;

    // default:
    //     break;
    // }
    coef->c[0] = dbl_to_fxd_p((dc/2.0), COEF_FR);
    coef->c[1] = dbl_to_fxd_p((-c/2.0), COEF_FR);
    
    // coef->c[0] = dbl_to_fxd_p(dc, COEF_FR);
    // coef->c[1] = dbl_to_fxd_p(-c, COEF_FR);
        // printf("c = %f\n", c);
        // printf("dc = %f\n", dc);
        // printf("d = %f\n", d);
        // printf("c = %f\n", c/2.0);
        // printf("dc = %f\n", dc/2.0);
        // printf("d = %f\n", d);
    coef->c_dbl[0] = dc;
    coef->c_dbl[1] = -c;
    if(form == 5){
        coef->c[0] = dbl_to_fxd_p(d, COEF_FR);
        coef->c[1] = dbl_to_fxd_p((-c), COEF_FR);
    }
    return 0;
}

static int32_t apf_coeff_1st_calc(apf_coefs_t *coef, double fc, double sample_rate){
    double c = (tan(M_PI *  fc / sample_rate) - 1) / (tan(M_PI *  fc / sample_rate) + 1);
    // c = -2.0;
    coef->c[0] = dbl_to_fxd_p(c, COEF_FR);
     printf("c = %f\n", c);
    coef->c_dbl[0] = c;
    return 0;
}
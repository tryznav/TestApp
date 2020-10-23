#include "cross_flt.h"

static double dB_to_coeff(double dB){
    return pow(10.0f , (0.05f * dB));
}

static int32_t convet_coefs(canon_doub_coefs_t *coeffs_dbl, canon_coefs_t *_coeffs);
static int32_t lpf_coeff_calc(canon_coefs_t *coeffs, double fc, double sample_rate);
static int32_t hpf_coeff_calc(canon_coefs_t *coeffs, double fc, double sample_rate);
static int32_t apf_coeff_1st_calc(apf_coef_1st_t *coef, double fc, double sample_rate);
static int32_t apf_coeff_2nd_calc(apf_coef_2nd_t *coef, double fc, double sample_rate);

int32_t cross_coeff_calc(cross_prm_t *prm, cross_coefs_t *coeffs){
    for(int i = 0; i < NUM_OF_FREQ; i++){
        // lpf_coeff_calc(&coeffs->lpf[i], prm->freq[i], prm->sample_rate);
        // hpf_coeff_calc(&coeffs->hpf[i], prm->freq[i], prm->sample_rate);
        apf_coeff_1st_calc(&coeffs->apf_1st[i], prm->freq[i], prm->sample_rate);
        apf_coeff_2nd_calc(&coeffs->apf_2nd[i], prm->freq[i], prm->sample_rate);
    }
    for(int i = 0; i < NUM_OF_BAND; i++){
        coeffs->gain_p[i] = (coef_type)dB_to_coeff(prm->gain_dB[i]);
    }
    return 0;
}

static int32_t lpf_coeff_calc(canon_coefs_t *coeffs, double fc, double sample_rate){
    canon_doub_coefs_t coeffs_dbl;
    double K = tan(M_PI *  fc / sample_rate);
    double Q = 1.0 / pow(2.0, 0.5);
    // fc -= (fc / Q) / 2; 

    double denominator = (Q + K + (Q *pow(K, 2.0)));
    // double denominator = (1.0 + (pow(2.0, 0.5) * K) +  pow(K, 2));

    coeffs_dbl.b0 = (pow(K, 2) * Q)/denominator;
    coeffs_dbl.b1 = (Q * 2.0 * pow(K, 2))/denominator;
    coeffs_dbl.b2 = coeffs_dbl.b0;

    coeffs_dbl.a1 = (Q * 2.0 * (pow(K, 2.0) - 1.0)) / denominator;
    coeffs_dbl.a2 = (Q - K + (Q * pow(K, 2))) / denominator;

    // coeffs_dbl.b0 = pow(K, 2)/denominator;
    // coeffs_dbl.b1 = (2.0 * pow(K, 2))/denominator;
    // coeffs_dbl.b2 = coeffs_dbl.b0;

    // coeffs_dbl.a1 = (2.0 * (pow(K, 2.0) - 1.0)) / denominator;
    // coeffs_dbl.a2 = (1.0 - (pow(2.0, 0.5) * K) +  pow(K, 2)) / denominator;


    convet_coefs(&coeffs_dbl, coeffs);
    return 0;
}

int32_t hpf_coeff_calc(canon_coefs_t *coeffs, double fc, double sample_rate){
    canon_doub_coefs_t coeffs_dbl;

    double K = tan(M_PI *  fc / sample_rate);
    double Q = 1.0 / pow(2.0, 0.5);
    // fc += (fc / Q) / 2; 
    // fc -= fc / Q; 

    // double denominator = (Q + K + (Q *pow(K, 2.0)));
    double denominator = (1.0 + (pow(2.0, 0.5) * K) +  pow(K, 2));


    coeffs_dbl.b0 = 1.0/denominator;
    coeffs_dbl.b1 = -2.0/denominator;
    coeffs_dbl.b2 = coeffs_dbl.b0;

    coeffs_dbl.a1 = (2.0 * (pow(K, 2) - 1)) / denominator;
    coeffs_dbl.a2 = (1.0 - (pow(2, 0.5) * K) + pow(K, 2.0)) / denominator;

    // coeffs_dbl.b0 = Q/denominator;
    // coeffs_dbl.b1 = -(Q * 2.0)/denominator;
    // coeffs_dbl.b2 = coeffs_dbl.b0;


    // coeffs_dbl.a1 = (Q * 2.0 * (pow(K, 2.0) - 1.0)) / denominator;
    // coeffs_dbl.a2 = (Q - K + ( Q * pow(K, 2))) / denominator;

    convet_coefs(&coeffs_dbl, coeffs);

    return 0;
}

static int32_t apf_coeff_2nd_calc(apf_coef_2nd_t *coef, double fc, double sample_rate){
    double Q = 1.0 / pow(2.0, 0.5);
    double fb = (fc / Q);
    double c = (tan(M_PI *  fb / sample_rate) - 1) / (tan(M_PI *  fb / sample_rate) + 1);
    double d = - cos(2 * M_PI  * fc / sample_rate);
    double dc = d * (1.0 - c);

    coef->c = (coef_type)c;
    coef->dc = (coef_type)dc;
    return 0;
}

static int32_t apf_coeff_1st_calc(apf_coef_1st_t *coef, double fc, double sample_rate){
    double c = (tan(M_PI *  fc / sample_rate) - 1) / (tan(M_PI *  fc / sample_rate) + 1);

    coef->c = (coef_type)c;
    return 0;
}

static int32_t convet_coefs(canon_doub_coefs_t *coeffs_dbl, canon_coefs_t *_coeffs){
    

    // coeffs_dbl->a1 = coeffs_dbl->a1 / coeffs_dbl->a0;
    // coeffs_dbl->a2 = coeffs_dbl->a2 / coeffs_dbl->a0;

    // coeffs_dbl->b0 = coeffs_dbl->b0 / coeffs_dbl->a0;
    // coeffs_dbl->b1 = coeffs_dbl->b1 / coeffs_dbl->a0;
    // coeffs_dbl->b2 =  coeffs_dbl->b2 /coeffs_dbl->a0;
    // coeffs_dbl->a0 = coeffs_dbl->a0  / coeffs_dbl->a0;


    _coeffs->a1 = (my_float)coeffs_dbl->a1;
    _coeffs->a2 = (my_float)coeffs_dbl->a2;
    _coeffs->b0 = (my_float)coeffs_dbl->b0;
    _coeffs->b1 = (my_float)coeffs_dbl->b1;
    _coeffs->b2 = (my_float)coeffs_dbl->b2;

    printf("_coeffs->a1 = %f\n", _coeffs->a1);
    printf("_coeffs->a2 = %f\n", _coeffs->a2);
    printf("_coeffs->b0 = %f\n", _coeffs->b0);
    printf("_coeffs->b1 = %f\n", _coeffs->b1);
    printf("_coeffs->b2 = %f\n\n", _coeffs->b2);
    

    // _coeffs->a1 = (my_float)coeffs_dbl->a1;
    // _coeffs->a2 = (my_float)coeffs_dbl->a2;

    // _coeffs->b0 = (my_float)coeffs_dbl->b0;
    // _coeffs->b1 = (my_float)coeffs_dbl->b1;
    // _coeffs->b2 = (my_float)coeffs_dbl->b2;

    return 0;
}

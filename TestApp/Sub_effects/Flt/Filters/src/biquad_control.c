#include "filters.h"
#include "filters_control.h"

/*******************************************************************************
 * Provides with the required data sizes for parameters and coefficients.
 *   It is caller responsibility to allocate enough memory (bytes) for them.
 * 
 * @param[out] params_bytes   required data size for storing parameters
 * @param[out] coeffs_bytes   required data size for storing coefficients
 * 
 * 
 ******************************************************************************/
void biquad_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes){
    *params_bytes = sizeof(biquad_coeffs_t);
    *params_bytes = sizeof(biquad_params_t);
}

/*******************************************************************************
 * Initializes params, coeffs and states to default values for the requested SR.
 * 
 * @param[in]  params       pointer to the pre-allocated params
 * @param[in]  coeffs       pointer to the pre-allocated coeffs
 * @param[in]  sample_rate  sampling rate
 * 
 ******************************************************************************/
void biquad_control_initialize(
    biquad_params_t*       params,
    biquad_coeffs_t*       coeffs,
    uint32_t               sample_rate){

    params->freq = 100.0;
    params->gain = 3.0;
    params->Q = 6.0;
    params->type = PQE;
    params->sample_rate = (double)sample_rate;
    params->Enable = 1;

    biquad_update_coeffs((biquad_params_t const *)params, coeffs);
}

/*******************************************************************************
 * Set single parameter 
 * 
 * @param[in] params    initialized params
 * @param[in] id        parameter ID
 * @param[in] value     parameter value
 * 
 * @return 0 if success, non-zero error code otherwise
 ******************************************************************************/
int32_t biquad_set_parameter(
    void*       params,
    int32_t     id,
    float       value){
    // id_union_t _id;
    // _id.all = id;
    // chain_prm_t *prm = (chain_prm_t *)params;

    //     switch (Id_prm.id.prm)
    // {
    // case 1:
    //     prm->freq = (double)value;
    //     break;
    // case 2:
    //    prm->gain = (double)value;
    //     break;
    // case 3:
    //     prm->Q = (double)value;
    //     break;
    // case 4:
    //     prm->type = (int32_t)value;
    //     break;
    // case Enable_id:
    //     prm->Enable = (int32_t)value;
    //     break;
    // default:
    //     break;
    // }

    // switch (_id.id.biquad_cascade)
    // {
    // case EQ1:

    //     break;
    // case Compresor_nb:
        
    //     break;
    // case EQ2:

    //     break;
    // case Limiter:

    //     break;
    // case Enable_id:
        
    //     break;
    // default:
    //     break;
    // }
    return 0;
}

/*******************************************************************************
 * Calculate coefficients, corresponding to the parameters.
 * 
 * @param[in] params    initialized params
 * @param[in] value     parameter value
 * 
 * @return 0 if success, non-zero error code otherwise
 ******************************************************************************/
void biquad_update_coeffs(
    biquad_params_t const* params,
    biquad_coeffs_t*       coeffs){
    double  A, sn, cs, alpha, beta, omega;
    double b0, b1, b2, a0, a1, a2;

    A = pow(10, params->gain / 40);
    omega = 2 * M_PI * params->freq / params->sample_rate;
    sn = sin(omega);
    cs = cos(omega);
    alpha = sn / (2 * params->Q);
    beta  = sqrt(A + A);


    // printf("prm->freq %f\n",   params->freq);
    // printf("prm->gain %f\n",   params->gain);
    // printf("prm->Q %f\n",      params->Q);
    // printf("prm->sample_rate %f\n", params->sample_rate);
    switch (params->type)
    {
    case LPF:
        b0 = (1.0 - cs) / 2.0;
        b1 =  1.0 - cs;
        b2 = (1.0 - cs) / 2.0;
        a0 =  1.0 + alpha;
        a1 = -2.0 * cs;
        a2 =  1.0 - alpha;
        break;
    case HPF:
        b0 =  (1.0 + cs) /2.0;
        b1 = -(1.0 + cs);
        b2 =  (1.0 + cs) /2.0;
        a0 =  1.0 + alpha;
        a1 = -2.0 * cs;
        a2 =  1.0 - alpha;
        break;
    case PQE:
        b0 =  1.0 + (alpha * A);
        b1 = -2.0 * cs;
        b2 =  1.0 - (alpha * A);
        a0 =  1.0 + (alpha / A);
        a1 = -2.0 * cs;
        a2 =  1.0 - (alpha / A);
        break;
    case LSH:
        b0 = A * ((A + 1) - (A - 1) * cs + beta * sn);
        b1 = A * ((A - 1) - (A + 1) * cs) * 2;
        b2 = A * ((A + 1) - (A - 1) * cs - beta * sn); 
        a0 =  (A + 1) + (A - 1) * cs + beta * sn;
        a1 = ((A - 1) + (A + 1) * cs) * (-2);
        a2 =  (A + 1) + (A - 1) * cs - beta * sn;
        break;
    case HSH:
        b0 = A * ((A + 1) + (A - 1) * cs + beta * sn);
        b1 = A * ((A - 1) + (A + 1) * cs) * (-2);
        b2 = A * ((A + 1) + (A - 1) * cs - beta * sn);
        a0 =  (A + 1) - (A - 1) * cs + beta * sn;
        a1 = ((A - 1) - (A + 1) * cs) * 2;
        a2 =  (A + 1) - (A - 1) * cs - beta * sn;
        break;
    }

    // a0[i] /= a0[i];
    b0 /= a0;
    b1 /= a0;
    b2 /= a0;
    a1 /= a0;
    a2 /= a0;

    if(!params->Enable){
        b0 = 1.0f;
        b1 = 0.0f;
        b2 = 0.0f;
        a0 = 0.0f;
        a1 = 0.0f;
        a2 = 0.0f;
    }

    for (size_t i = 0; i < 2; i++) {
        coeffs->a1.ch[i] = (mfloat)(-a1);
        coeffs->a2.ch[i] = (mfloat)(-a2);
        coeffs->b0.ch[i] = (mfloat)(b0);
        coeffs->b1.ch[i] = (mfloat)(b1);
        coeffs->b2.ch[i] = (mfloat)(b2);
    }
    for (size_t i = 2; i < 4; i++) {
        coeffs->a1.ch[i] = 0.0f;
        coeffs->a2.ch[i] = 0.0f;
        coeffs->b0.ch[i] = 1.0f;
        coeffs->b1.ch[i] = 0.0f;
        coeffs->b2.ch[i] = 0.0f;
    }
}

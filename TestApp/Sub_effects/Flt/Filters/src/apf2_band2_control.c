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
void apf2_band2_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes){

    *params_bytes = sizeof(apf2_band2_coeffs_t);
    *params_bytes = sizeof(apf2_band2_params_t);

}

/*******************************************************************************
 * Initializes params, coeffs and states to default values for the requested SR.
 * 
 * @param[in]  params       pointer to the pre-allocated params
 * @param[in]  coeffs       pointer to the pre-allocated coeffs
 * @param[in]  sample_rate  sampling rate
 * 
 ******************************************************************************/
void apf2_band2_control_initialize(
    apf2_band2_params_t*       params,
    apf2_band2_coeffs_t*       coeffs,
    uint32_t                        sample_rate){

    for(int i = 0; i < 2; i++){
        params->freq[i] = 100.0 + (i * 200);
    }
    params->sample_rate = (double)sample_rate;

    apf2_band2_update_coeffs((apf2_band2_params_t const*)params, coeffs);
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
int32_t apf2_band2_set_parameter(
    void*       params,
    int32_t     id,
    float       value){
    // id_union_t _id;
    // _id.all = id;
    // chain_prm_t *prm = (chain_prm_t *)params;

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
void apf2_band2_update_coeffs(
    apf2_band2_params_t const* params,
    apf2_band2_coeffs_t*       coeffs){
    double Q = 0.70710678118;
    double fb[2], c[2], d[2] , dc[2];

    for(uint32_t i = 0;  i < 2; i++){
        fb[i] =  (params->freq[i] / Q);
        c[i] = (tan(M_PI *  fb[i] / params->sample_rate) - 1) / (tan(M_PI *  fb[i] / params->sample_rate) + 1);
        d[i] = -cos(2 * M_PI  * params->freq[i] / params->sample_rate);
        dc[i] = d[i] * (1.0 - c[i]);
    }

    for(uint32_t i = 0;  i < 2; i++){
        coeffs->k1.ch[i] = (mfloat)dc[0];
        coeffs->k2.ch[i] = (mfloat)-c[0];
    }

    for(uint32_t i = 2;  i < 4; i++){
        coeffs->k1.ch[i] = (mfloat)dc[1];
        coeffs->k2.ch[i] = (mfloat)-c[1];
    }
}

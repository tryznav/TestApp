#include "cross_flt.h"

/*******************************************************************************
 * Provides with the required data sizes for parameters and coefficients.
 *   It is caller responsibility to allocate enough memory (bytes) for them.
 * 
 * @param[out] params_bytes   required data size for storing parameters
 * @param[out] coeffs_bytes   required data size for storing coefficients
 * 
 * @return 0 if success, non-zero error code otherwise
 ******************************************************************************/
int32_t cross_flt_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes){
    *params_bytes = sizeof(cross_prm_t);
    *coeffs_bytes = sizeof(coef_t);
    return 0;
}

/*******************************************************************************
 * Initializes params, coeffs and states to default values for the requested SR.
 * 
 * @param[in]  params       pointer to the pre-allocated params
 * @param[in]  coeffs       pointer to the pre-allocated coeffs
 * @param[in]  sample_rate  sampling rate
 * 
 * @return 0 if effect is initialized, non-zero error code otherwise
 ******************************************************************************/
int32_t cross_flt_control_initialize(
    void*       params,
    void*       coeffs,
    uint32_t    sample_rate){
    cross_prm_t *_prm = (cross_prm_t *)params;
    coef_t *_coeffs = (coef_t  *)coeffs;

    _prm->sample_rate = (double)sample_rate;

    _prm->freq[0] = 10000;
    _prm->freq[1] = 10000;
    _prm->freq[2] = 20000;
    _prm->gain_dB[0] = -6.0;
    _prm->gain_dB[1] = -6.0;
    _prm->gain_dB[2] = -6.0;
    _prm->gain_dB[3] = -6.0;

    cross_coeff_calc(_prm, _coeffs);

    return 0;
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
int32_t cross_flt_set_parameter(
    void*       params,
    int32_t     id,
    float       value){
    cross_prm_t *_prm = (cross_prm_t *)params;

    switch (id)
    {
    case  PRM_SAMPLE_RATE_ID:
        _prm->sample_rate = value;
        return 0;
    case  PRM_CROSSOVER_F0_ID:
        _prm->freq[0] = (double)value;
        return 0;
    case  PRM_CROSSOVER_F1_ID:
        _prm->freq[1] = (double)value;
        return 0;
    case  PRM_CROSSOVER_F2_ID:
        _prm->freq[2] = (double)value;
        return 0;
    case  PRM_CROSSOVER_G0_ID:
        _prm->gain_dB[0] = (double)value;
        return 0;
    case  PRM_CROSSOVER_G1_ID:
        _prm->gain_dB[1] = (double)value;
        return 0;
    case  PRM_CROSSOVER_G2_ID:
        _prm->gain_dB[2] = (double)value;
        return 0;
    default:
        fprintf(stderr, RED"Error: "BOLDWHITE"Unsupported params. Rejected.\n"RESET);
        return -1;
    }
}

/*******************************************************************************
 * Calculate coefficients, corresponding to the parameters.
 * 
 * @param[in] params    initialized params
 * @param[in] value     parameter value
 * 
 * @return 0 if success, non-zero error code otherwise
 ******************************************************************************/
int32_t cross_flt_update_coeffs(
    void const* params,
    void*       coeffs){

    cross_coeff_calc((cross_prm_t *)params, (coef_t  *)coeffs);

    return 0;
}

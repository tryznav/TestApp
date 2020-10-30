#include "apf_fxd.h"

/*******************************************************************************
 * Provides with the required data sizes for parameters and coefficients.
 *   It is caller responsibility to allocate enough memory (bytes) for them.
 * 
 * @param[out] params_bytes   required data size for storing parameters
 * @param[out] coeffs_bytes   required data size for storing coefficients
 * 
 * @return 0 if success, non-zero error code otherwise
 ******************************************************************************/
int32_t apf_fxd_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes){
    *params_bytes = sizeof(apf_prm_t);
    *coeffs_bytes = sizeof(apf_coefs_t);
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
int32_t apf_fxd_control_initialize(
    void*       params,
    void*       coeffs,
    uint32_t    sample_rate){
    apf_prm_t *_prm = (apf_prm_t *)params;
    apf_coefs_t *_coeffs = (apf_coefs_t  *)coeffs;

    _prm->sample_rate = (double)sample_rate;
    _prm->freq = 1.0; //min cutoff freq
    _prm->form = 1;
    _prm->order = 1;
    apf_coeff_calc_fxd(_prm, _coeffs);

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
int32_t apf_fxd_set_parameter(
    void*       params,
    int32_t     id,
    float       value){
    apf_prm_t *_prm = (apf_prm_t *)params;

    switch (id)
    {
    case  PRM_SAMPLE_RATE_ID:
        _prm->sample_rate = (double)value;
        return 0;
    case  PRM_FREQ_END_ID:
        _prm->freq = (double)value;
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
int32_t apf_fxd_update_coeffs(
    void const* params,
    void*       coeffs){

    apf_coeff_calc_fxd((apf_prm_t *)params, (apf_coefs_t  *)coeffs);


    return 0;
}

#include "iir_flt.h"


/*******************************************************************************
 * Provides with the required data sizes for parameters and coefficients.
 *   It is caller responsibility to allocate enough memory (bytes) for them.
 * 
 * @param[out] params_bytes   required data size for storing parameters
 * @param[out] coeffs_bytes   required data size for storing coefficients
 * 
 * @return 0 if success, non-zero error code otherwise
 ******************************************************************************/
int32_t iir_flt_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes){
    *params_bytes = sizeof(iir_prm_t);
    *coeffs_bytes = sizeof(float) * TAP_NUM;
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
int32_t iir_flt_control_initialize(
    void*       params,
    void*       coeffs,
    uint32_t    sample_rate){
    iir_prm_t *_prm = (iir_prm_t *)params;
    iir_coefs_t *_coeffs = (iir_coefs_t  *)coeffs;

    _prm->sample_rate= (double)sample_rate;
    _prm->cutoff_freq.sweep.start = (float)(_prm->sample_rate * 0.05);
    _prm->cutoff_freq.sweep.end   = (float)(_prm->sample_rate * 0.1);

    iir_coeff_calc(_prm, _coeffs);

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
int32_t iir_flt_set_parameter(
    void*       params,
    int32_t     id,
    float       value){
    iir_prm_t *_prm = (iir_prm_t *)params;

    switch (id)
    {
    case  PRM_SAMPLE_RATE_ID:
        _prm->sample_rate = value;
        return 0;
    case  PRM_FREQ_START_ID:
        _prm->cutoff_freq.sweep.start = value;
        return 0;
    case  PRM_FREQ_END_ID:
        _prm->cutoff_freq.sweep.end = value;
        return 0;
    case PRM_GAIN_dB_ID:
        _prm->gain_dB = value;
        return 0;
    case BPF:
        _prm->type =  BPF;
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
int32_t iir_flt_update_coeffs(
    void const* params,
    void*       coeffs){

    iir_coeff_calc((iir_prm_t *)params, (iir_coefs_t  *)coeffs);

    return 0;
}

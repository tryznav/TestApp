#include "iir_flt.h"

static int32_t iir_coeff_calc(iir_prm_t *_prm, iir_coefs_t *_coeffs){
    iir_doub_coefs_t coeffs_dbl;

    _prm->BW = (double)(_prm->cutoff_freq.sweep.end - _prm->cutoff_freq.sweep.start);
    _prm->f0 =  (double)(_prm->cutoff_freq.sweep.end + _prm->cutoff_freq.sweep.start) / 2.0;

    _prm->Q = _prm->f0 / _prm->BW;

    _prm->w = 2 * M_PI * _prm->f0 / _prm->sample_rate;
    _prm->alpha = sin(_prm->w)/(2.0 * _prm->Q);



    coeffs_dbl.a0 = 1.0 + _prm->alpha;
    coeffs_dbl.a1 = -2.0 * cos(_prm->w);
    coeffs_dbl.a2 = 1.0 - _prm->alpha;

    coeffs_dbl.b0 = _prm->alpha;
    coeffs_dbl.b1 = 0.0f;
    coeffs_dbl.b2 = -_prm->alpha;

    
    coeffs_dbl.a1 = coeffs_dbl.a1 / coeffs_dbl.a0;
    coeffs_dbl.a2 = coeffs_dbl.a2 / coeffs_dbl.a0;

    coeffs_dbl.b0 = coeffs_dbl.b0 / coeffs_dbl.a0;
    coeffs_dbl.b1 = 0.0f;
    coeffs_dbl.b2 =  coeffs_dbl.b2 /coeffs_dbl.a0;
    coeffs_dbl.a0 = coeffs_dbl.a0  / coeffs_dbl.a0;


    _coeffs->a0 = (my_float)coeffs_dbl.a0;
    _coeffs->a1 = (my_float)coeffs_dbl.a1;
    _coeffs->a2 = (my_float)coeffs_dbl.a2;

    // printf("a0 = %f\n", _coeffs->a0);
    // printf("a1 = %f\n", _coeffs->a1);
    // printf("a2 = %f\n", _coeffs->a2);

    _coeffs->b0 = (my_float)coeffs_dbl.b0;
    _coeffs->b1 = (my_float)coeffs_dbl.b1;
    _coeffs->b2 = (my_float)coeffs_dbl.b2;

    // printf("b0 = %f\n", _coeffs->b0);
    // printf("b1 = %f\n", _coeffs->b1);
    // printf("b2 = %f\n", _coeffs->b2);


    return 0;
}

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

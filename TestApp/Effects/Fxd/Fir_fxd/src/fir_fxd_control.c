#include "fir_fxd_control.h"
#include "fxd_arithmetic.h"

#include "colors.h"
#include "string.h"
// #include "fxd_arithmetic.h"

#include <stdio.h>
#include <math.h>

#define TAP_NUM     256
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

typedef struct sweep_s{
    float               start;
    float               end;
}sweep_t;

union feature{
    float               whole_file;
    sweep_t             sweep;
};

typedef struct lpf_prm_s{
    float               sample_rate;
    union feature       cutoff_freq;
}lpf_prm_t;

static int32_t bpf_coeff_calc(fxd_q31_t *coeff, union feature cutoff_freq, float sample_rate){
    for (int i = 0; i <  TAP_NUM; i++ ){
        ((fxd_q31_t *)coeff)[i] = dbl_to_fxd_p(bpf_coef[i], COEF_FRACTIONAL_BITS);// (fxd_q31_t)(bpf_coef[i] * (1 << COEF_FRACTIONAL_BITS));
    }
    return  0;
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
int32_t fir_fxd_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes){
    *params_bytes = sizeof(lpf_prm_t);
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
int32_t fir_fxd_control_initialize(
    void*       params,
    void*       coeffs,
    uint32_t    sample_rate){
    lpf_prm_t *_prm = (lpf_prm_t *)params;

    _prm->sample_rate = (float)sample_rate;
    _prm->cutoff_freq.sweep.start = _prm->sample_rate * 0.05f;
    _prm->cutoff_freq.sweep.end   = _prm->sample_rate * 0.1f;

    bpf_coeff_calc((fxd_q31_t *)coeffs,  _prm->cutoff_freq, _prm->sample_rate);

    for (int i = 0; i <  TAP_NUM; i++ ){
        printf("coeffs[%d] = %d\n", i, ((fxd_q31_t *)coeffs)[i]);
    }
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
int32_t fir_fxd_set_parameter(
    void*       params,
    int32_t     id,
    float       value){
    int32_t n = -1;

    lpf_prm_t *_prm = (lpf_prm_t *)params;

    if(id == PRM_SAMPLE_RATE_ID){
        _prm->sample_rate = value;
        n = 0;
    }
    if(id == PRM_FREQ_START_ID){
        _prm->cutoff_freq.sweep.start = value;
        n = 0;
    }
    if(id == PRM_FREQ_END_ID){
        _prm->cutoff_freq.sweep.end = value;
        n = 0;
    }
    printf("fir_fxd_set_parameter\n");
    if(n != 0){
        fprintf(stderr, RED"Error: "BOLDWHITE"Unsupported params. Rejected.\n"RESET);
    }
    return n;
}

/*******************************************************************************
 * Calculate coefficients, corresponding to the parameters.
 * 
 * @param[in] params    initialized params
 * @param[in] value     parameter value
 * 
 * @return 0 if success, non-zero error code otherwise
 ******************************************************************************/
int32_t fir_fxd_update_coeffs(
    void const* params,
    void*       coeffs){
    lpf_prm_t *_prm = (lpf_prm_t *)params;

    bpf_coeff_calc((fxd_q31_t *)coeffs,  _prm->cutoff_freq, _prm->sample_rate);

    for (int i = 0; i <  TAP_NUM; i++ ){
        printf("coeffs[%d] = %d\n", i, ((fxd_q31_t *)coeffs)[i]);
    }

    printf("fir_fxd_update_coeffs\n");
    return 0;
}

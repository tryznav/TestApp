#include "effect_control.h"
#include "params_id.h"
#include "colors.h"
#include "fxd_arithmetic.h"

#include <stdio.h>
#include <math.h>

#define TAP_NUM     256
#define M_PI 3.14159265358979323846

typedef struct lpf_prm_s{
    uint32_t    sample_rate;
    uint32_t    cutoff_freq;
}lpf_prm_t;

static int32_t coeff_calc(my_float *coeff, uint32_t cutoff_freq, uint32_t sample_rate){

    memset(coeff, 0, sizeof(double) * TAP_NUM);
    int nm = (TAP_NUM - 1) / 2;                                 //номера значений для коэфф фильтра симметричны
    int j = nm;                                                 //отсчет от середины фильтра

    for (int i = 0; i < nm; i++) {
        if (i == 0)
        {
            coeff[j] = 2.0 * (double)cutoff_freq / sample_rate;
        }
        else
        {
            coeff[j + i] = (float)(sin(2.0 * M_PI * i * cutoff_freq / sample_rate))  / (M_PI * i);
            coeff[j - i] = coeff[j + i];
        }
    }
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
int32_t fir_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes){
    *params_bytes = sizeof(lpf_prm_t);
    *coeffs_bytes = sizeof(fxd_q31_t) * TAP_NUM;
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
int32_t fir_control_initialize(
    void*       params,
    void*       coeffs,
    uint32_t    sample_rate){
    lpf_prm_t *_prm = (lpf_prm_t *)params;

    _prm->sample_rate = sample_rate;
    _prm->cutoff_freq = sample_rate/200;
    coeff_calc((fxd_q31_t *)coeffs,  _prm->cutoff_freq, _prm->sample_rate);

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
int32_t fir_set_parameter(
    void*       params,
    int32_t     id,
    float       value){

    lpf_prm_t *_prm = (lpf_prm_t *)params;

    if(id == PRM_SAMPLE_RATE_ID){
        _prm->sample_rate = value;
        return 0;
    }
    if(id == PRM_FREQ_START_ID){
        _prm->cutoff_freq = value;
        return 0;
    }

    fprintf(stderr, RED"Error: "BOLDWHITE"Unsupported params. Rejected.\n"RESET);
    return -1;
}

/*******************************************************************************
 * Calculate coefficients, corresponding to the parameters.
 * 
 * @param[in] params    initialized params
 * @param[in] value     parameter value
 * 
 * @return 0 if success, non-zero error code otherwise
 ******************************************************************************/
int32_t fir_update_coeffs(
    void const* params,
    void*       coeffs){
    lpf_prm_t *_prm = (lpf_prm_t *)params;

    coeff_calc((fxd_q31_t *)coeffs,  _prm->cutoff_freq, _prm->sample_rate);
    return 0;
}

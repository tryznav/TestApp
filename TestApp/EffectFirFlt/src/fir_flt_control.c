#include "fir_flt_control.h"
// #include "params_id.h"
#include "colors.h"
#include "string.h"
// #include "fxd_arithmetic.h"

#include <stdio.h>
#include <math.h>

#define TAP_NUM     256
#define M_PI 3.14159265358979323846

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

static int32_t bpf_coeff_calc(float *coeff, union feature cutoff_freq, float sample_rate){
    for (int i = 0; i <  TAP_NUM; i++ ){
        ((float *)coeff)[i] = (float)bpf_coef[i];
    }
    return  0;
}
//     // printf()

//     // memset(coeff, 0, sizeof(double) * TAP_NUM);
//     // for(int i = 0; i < TAP_NUM; i++){
//     //     double tmp = (1.0 / ((double)i - 0.5) * M_PI) * sin( 2 * M_PI )* ((double)i - 0.5) * (double)cutoff_freq.sweep.end;
//     //       printf("cutoff_freq.sweep.end[%d] = %f\n", i, cutoff_freq.sweep.end);
//     //     tmp -=  sin( 2 * M_PI )* ((double)i - 0.5) *cutoff_freq.sweep.start;
//     //     printf("tmp[%d] = %f\n", i, tmp);
//     //     coeff[i] = (float)tmp;
//     // }
//     for (int i = 0; i <  TAP_NUM; i++ ){
//         ((float *)coeff)[i] = 0;
//     }

//     int nm = (TAP_NUM - 1) / 2;                                 //номера значений для коэфф фильтра симметричны
//     int j = nm;                                                 //отсчет от середины фильтра

//     for (int i = 0; i < nm; i++) {
//         if (i == 0)
//         {
//             coeff[j] = 2.0f * (cutoff_freq.sweep.end - cutoff_freq.sweep.start);
//             // printf("coeff[%d] = %f\n",  j, coeff[j]);
//         }
//         else
//         {
//             coeff[j + i] = (float)(1.0/(M_PI * (double)i) * (sin(2.0 * M_PI * (double)i *(double)cutoff_freq.sweep.end))); //- sin(2.0 * M_PI * (double)i *(double)cutoff_freq.sweep.start)));
//             coeff[j - i] = coeff[j + i];
//             printf("coeff[%d] = %f\n",  j, coeff[j + i]);
//         }
//     }
//     // float acom = 0;
//     // for (int i = 0; i <  TAP_NUM; i++ ){
//     //     acom += ((float *)coeff)[i];
//     // }
//     for (int i = 0; i <  TAP_NUM; i++ ){
//         //  printf("coeffs[%d] = %f\n", i, ((float *)coeff)[i]);
//         // ((float *)coeff)[i] = ((float *)coeff)[i] / acom;
       
//     }
//     // printf("acom %f\n", acom);
//     return 0;
// }

/*******************************************************************************
 * Provides with the required data sizes for parameters and coefficients.
 *   It is caller responsibility to allocate enough memory (bytes) for them.
 * 
 * @param[out] params_bytes   required data size for storing parameters
 * @param[out] coeffs_bytes   required data size for storing coefficients
 * 
 * @return 0 if success, non-zero error code otherwise
 ******************************************************************************/
int32_t fir_flt_control_get_sizes(
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
int32_t fir_flt_control_initialize(
    void*       params,
    void*       coeffs,
    uint32_t    sample_rate){
    lpf_prm_t *_prm = (lpf_prm_t *)params;

    _prm->sample_rate = (float)sample_rate;
    // _prm->cutoff_freq.sweep.start = _prm->sample_rate * 0.05f;
    // _prm->cutoff_freq.sweep.end   = _prm->sample_rate * 0.1f;
    bpf_coeff_calc((float *)coeffs,  _prm->cutoff_freq, _prm->sample_rate);

    printf("all");
    for (int i = 0; i <  TAP_NUM; i++ ){
        printf("coeffs[%d] = %f\n", i, ((float *)coeffs)[i]);
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
int32_t fir_flt_set_parameter(
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
    printf("fir_flt_set_parameter\n");
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
int32_t fir_flt_update_coeffs(
    void const* params,
    void*       coeffs){
    lpf_prm_t *_prm = (lpf_prm_t *)params;

    bpf_coeff_calc((float *)coeffs,  _prm->cutoff_freq, _prm->sample_rate);
    for (int i = 0; i <  TAP_NUM; i++ ){
        printf("coeffs[%d] = %f\n", i, ((float *)coeffs)[i]);
    }
    // memcpy(coeffs, bpf_coef, (sizeof(float) * 256));
    printf("fir_flt_update_coeffs\n");
    return 0;
}

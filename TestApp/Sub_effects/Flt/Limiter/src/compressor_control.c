#include "compressor_4band.h"
#include "compressor_4band_control.h"

/*******************************************************************************
 * Provides with the rcompressor_4banduired data sizes for parameters and coefficients.
 *   It is caller responsibility to allocate enough memory (bytes) for them.
 * 
 * @param[out] params_bytes   rcompressor_4banduired data size for storing parameters
 * @param[out] coeffs_bytes   rcompressor_4banduired data size for storing coefficients
 * 
 * 
 ******************************************************************************/
void compressor_4band_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes){
    *params_bytes = sizeof(compressor_4band_coeffs_t);
    *params_bytes = sizeof(compressor_4band_params_t);
}

/*******************************************************************************
 * Initializes params, coeffs and states to default values for the rcompressor_4banduested SR.
 * 
 * @param[in]  params       pointer to the pre-allocated params
 * @param[in]  coeffs       pointer to the pre-allocated coeffs
 * @param[in]  sample_rate  sampling rate
 * 
 ******************************************************************************/
void compressor_4band_control_initialize(
    compressor_4band_params_t*         params,
    compressor_4band_coeffs_t*         coeffs,
    uint32_t                sample_rate){
    crossover4b_control_initialize(&params->crossover, &coeffs->crossover, sample_rate);
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
int32_t compressor_4band_set_parameter(
    void*       params,
    int32_t     id,
    float       value){
    // id_union_t _id;
    // _id.all = id;
    // compressor_4band_prm_t *prm = (compressor_4band_prm_t *)params;

    // switch (_id.id.biquad_cascade)
    // {
    // case compressor_4band1:

    //     break;
    // case Compresor_nb:
        
    //     break;
    // case compressor_4band2:

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
 ******************************************************************************/
void compressor_4band_update_coeffs(
    compressor_4band_params_t const* params,
    compressor_4band_coeffs_t*       coeffs){
    crossover4b_update_coeffs((crossover4b_params_t const*)&params->crossover, &coeffs->crossover);
}
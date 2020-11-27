#include "chain_api.h"

/*******************************************************************************
 * Provides with the required data sizes for parameters and coefficients.
 *   It is caller responsibility to allocate enough memory (bytes) for them.
 * 
 * @param[out] params_bytes   required data size for storing parameters
 * @param[out] coeffs_bytes   required data size for storing coefficients
 * 
 * @return 0 if success, non-zero error code otherwise
 ******************************************************************************/
int32_t biquad_cascade_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes){

    return 0;
}

/*******************************************************************************
 * Initializes params, coeffs and states to default values for the requested SR.
 * 
 * @param[in]  params       pointer to the pre-allocated params
 * @param[in]  coeffs       pointer to the pre-allocated coeffs
 * @param[in]  sample_rate  sampling rate
 * 
 * @return 0 if biquad_cascade is initialized, non-zero error code otherwise
 ******************************************************************************/
int32_t biquad_cascade_control_initialize(
    void*       params,
    void*       coeffs,
    uint32_t    sample_rate){

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
int32_t biquad_cascade_set_parameter(
    void*       params,
    int32_t     id,
    float       value){
    id_union_t _id;
    _id.all = id;
    chain_prm_t *prm = (chain_prm_t *)params;

    switch (_id.id.biquad_cascade)
    {
    case EQ1:

        break;
    case Compresor_nb:
        
        break;
    case EQ2:

        break;
    case Limiter:

        break;
    case Enable_id:
        
        break;
    default:
        break;
    }
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
int32_t biquad_cascade_update_coeffs(
    void const* params,
    void*       coeffs){
  
  

    return 0;
}

#include "chain.h"
#include "chain_control.h"

/*******************************************************************************
 * Provides with the rchainuired data sizes for parameters and coefficients.
 *   It is caller responsibility to allocate enough memory (bytes) for them.
 * 
 * @param[out] params_bytes   rchainuired data size for storing parameters
 * @param[out] coeffs_bytes   rchainuired data size for storing coefficients
 * 
 * 
 ******************************************************************************/
void chain_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes){
    *params_bytes = sizeof(chain_coeffs_t);
    *params_bytes = sizeof(chain_params_t);
}

/*******************************************************************************
 * Initializes params, coeffs and states to default values for the rchainuested SR.
 * 
 * @param[in]  params       pointer to the pre-allocated params
 * @param[in]  coeffs       pointer to the pre-allocated coeffs
 * @param[in]  sample_rate  sampling rate
 * 
 ******************************************************************************/
void chain_control_initialize(
    chain_params_t*         params,
    chain_coeffs_t*         coeffs,
    uint32_t                sample_rate){

    eq_control_initialize(&params->eq1, &coeffs->eq1, sample_rate);
    compressor_4band_control_initialize(&params->compressor, &coeffs->compressor, sample_rate);

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
int32_t chain_set_parameter(
    chain_params_t*       params,
    int32_t               id,
    float                 value){
    int32_t res = 0;
    id_union_t _id;
    _id.id = id;

    switch (_id.effect)
    {
    case 0://EQ
        res = eq_set_parameter(&params->eq1, id, value);
        break;
    case 1://M COMPRESSOR
        res = compressor_4band_set_parameter(&params->compressor, id, value);
        break;
    case 3://LIMITER

        break;
    case 255: //ENABLE

        break;
    default:
        break;
    }
    return res;
}

/*******************************************************************************
 * Calculate coefficients, corresponding to the parameters.
 * 
 * @param[in] params    initialized params
 * @param[in] value     parameter value
 * 
 ******************************************************************************/
void chain_update_coeffs(
    chain_params_t const* params,
    chain_coeffs_t*       coeffs){
    eq_update_coeffs((eq_params_t const*)&params->eq1, &coeffs->eq1);
    compressor_4band_update_coeffs((compressor_4band_params_t const*)&params->compressor, &coeffs->compressor);
}

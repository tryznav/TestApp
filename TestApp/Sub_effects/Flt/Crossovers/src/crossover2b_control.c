#include "crossovers.h"
#include "crossovers_control.h"

/*******************************************************************************
 * Provides with the required data sizes for parameters and coefficients.
 *   It is caller responsibility to allocate enough memory (bytes) for them.
 * 
 * @param[out] params_bytes   required data size for storing parameters
 * @param[out] coeffs_bytes   required data size for storing coefficients
 * 
 * 
 ******************************************************************************/
void crossover2b_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes){

    *params_bytes = sizeof(crossover2b_coeffs_t);
    *params_bytes = sizeof(crossover2b_params_t);

}

/*******************************************************************************
 * Initializes params, coeffs and states to default values for the requested SR.
 * 
 * @param[in]  params       pointer to the pre-allocated params
 * @param[in]  coeffs       pointer to the pre-allocated coeffs
 * @param[in]  sample_rate  sampling rate
 * 
 ******************************************************************************/
void crossover2b_control_initialize(
    crossover2b_params_t*       params,
    crossover2b_coeffs_t*       coeffs,
    uint32_t                        sample_rate){
    
    params->sample_rate = (double)sample_rate;
    params->freq = params->sample_rate/16.0;

    crossover2b_update_coeffs((crossover2b_params_t const*)params, coeffs);
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
int32_t crossover2b_set_parameter(
    void*       params,
    int32_t     id,
    float       value){
    // id_union_t _id;
    // _id.all = id;
    // chain_prm_t *prm = (chain_prm_t *)params;

    // switch (_id.id.biquad_cascade)
    // {
    // case EQ1:

    //     break;
    // case Compresor_nb:
        
    //     break;
    // case EQ2:

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
 * @return 0 if success, non-zero error code otherwise
 ******************************************************************************/
void crossover2b_update_coeffs(
    crossover2b_params_t const* params,
    crossover2b_coeffs_t*       coeffs){
    double Q = 0.70710678118;
    double fb = (params->freq / Q);

    double c = (tan(M_PI *  fb / params->sample_rate) - 1) / (tan(M_PI *  fb / params->sample_rate) + 1);
    double d = -cos(2 * M_PI  * params->freq / params->sample_rate);
    double dc = d * (1.0 - c);

    double c1 = (tan(M_PI * params->freq / params->sample_rate) - 1) / (tan(M_PI *  params->freq / params->sample_rate) + 1);
//Change if use 2ch
    for(uint32_t i = 0; i < FOUR; i++){
        coeffs->k1.ch[i] =  (mfloat)dc;
        coeffs->k2.ch[i] =  (mfloat)-c;
        coeffs->k.ch[i] =   (mfloat)c1;
        coeffs->half.ch[i] = (mfloat)0.5;
    }
}

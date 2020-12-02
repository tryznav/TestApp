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
void crossover4b_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes){

    *params_bytes = sizeof(crossover4b_coeffs_t);
    *params_bytes = sizeof(crossover4b_params_t);

}

/*******************************************************************************
 * Initializes params, coeffs and states to default values for the requested SR.
 * 
 * @param[in]  params       pointer to the pre-allocated params
 * @param[in]  coeffs       pointer to the pre-allocated coeffs
 * @param[in]  sample_rate  sampling rate
 * 
 ******************************************************************************/
void crossover4b_control_initialize(
    crossover4b_params_t*       params,
    crossover4b_coeffs_t*       coeffs,
    uint32_t                    sample_rate){

    params->apf_compens.freq[0] = 10000.0;
    params->apf_compens.freq[1] = 200.0;
    params->apf_compens.sample_rate = (double)sample_rate;

    params->crossover2b_to_4b.freq[0] = 200.0;
    params->crossover2b_to_4b.freq[1] = 10000.0;
    params->crossover2b_to_4b.sample_rate = (double)sample_rate;

    params->crossover2b.freq = 1000.0;
    params->crossover2b.sample_rate = (double)sample_rate;

    crossover4b_update_coeffs((crossover4b_params_t const*)params, coeffs);
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
int32_t crossover4b_set_parameter(
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
void crossover4b_update_coeffs(
    crossover4b_params_t const* params,
    crossover4b_coeffs_t*       coeffs){

    crossover2b_update_coeffs((crossover2b_params_t const*)&params->crossover2b, &coeffs->crossover2b);
    apf2_band2_update_coeffs((apf2_band2_params_t const*)&params->apf_compens, &coeffs->apf_compens);
    crossover2b_to_4b_update_coeffs((crossover2b_to_4b_params_t const*)&params->crossover2b_to_4b, &coeffs->crossover2b_to_4b);
}

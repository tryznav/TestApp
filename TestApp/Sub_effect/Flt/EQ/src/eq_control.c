#include "eq.h"
#include "eq_control.h"

/*******************************************************************************
 * Provides with the required data sizes for parameters and coefficients.
 *   It is caller responsibility to allocate enough memory (bytes) for them.
 * 
 * @param[out] params_bytes   required data size for storing parameters
 * @param[out] coeffs_bytes   required data size for storing coefficients
 * 
 * 
 ******************************************************************************/
void eq_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes){
    *params_bytes = sizeof(eq_coeffs_t);
    *params_bytes = sizeof(eq_params_t);
}

/*******************************************************************************
 * Initializes params, coeffs and states to default values for the requested SR.
 * 
 * @param[in]  params       pointer to the pre-allocated params
 * @param[in]  coeffs       pointer to the pre-allocated coeffs
 * @param[in]  sample_rate  sampling rate
 * 
 ******************************************************************************/
void eq_control_initialize(
    eq_params_t*       params,
    eq_coeffs_t*       coeffs,
    uint32_t           sample_rate){

    for(uint32_t i = 0; i < 2; i++){
        for(uint32_t y = 0; y < 4; y++){
            params->cascade4[i].cascade4[y].freq = 100.0 + (i * 1000) + (y * 100);
            params->cascade4[i].cascade4[y].gain = 3.0;
            params->cascade4[i].cascade4[y].Q = 6.0;
            params->cascade4[i].cascade4[y].type = PQE;
            params->cascade4[i].cascade4[y].sample_rate = (double)sample_rate;
            params->cascade4[i].cascade4[y].Enable = 1;
        }
    }
    for(uint32_t i = 0; i < 2; i++){
        params->cascade2.cascade2[i].freq = 3000 + (i * 1000);
        params->cascade2.cascade2[i].gain = 3.0;
        params->cascade2.cascade2[i].Q = 6.0;
        params->cascade2.cascade2[i].type = PQE;
        params->cascade2.cascade2[i].sample_rate = (double)sample_rate;
        params->cascade2.cascade2[i].Enable = 1;
    }

    eq_update_coeffs((eq_params_t const *)params, coeffs);
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
int32_t eq_set_parameter(
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
    // return 0;
}

/*******************************************************************************
 * Calculate coefficients, corresponding to the parameters.
 * 
 * @param[in] params    initialized params
 * @param[in] value     parameter value
 * 
 ******************************************************************************/
void eq_update_coeffs(
    eq_params_t const* params,
    eq_coeffs_t*       coeffs){

    for(uint32_t i = 0; i < 2; i++){
        biquad_cascade4_update_coeffs(&params->cascade4[i], &coeffs->cascade4[i]);
    }
    biquad_cascade2_update_coeffs(&params->cascade2, &coeffs->cascade2);
}

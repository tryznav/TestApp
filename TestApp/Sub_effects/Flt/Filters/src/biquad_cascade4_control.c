#include "filters.h"
#include "filters_control.h"

/*******************************************************************************
 * Provides with the required data sizes for parameters and coefficients.
 *   It is caller responsibility to allocate enough memory (bytes) for them.
 * 
 * @param[out] params_bytes   required data size for storing parameters
 * @param[out] coeffs_bytes   required data size for storing coefficients
 * 
 * 
 ******************************************************************************/
void biquad_cascade4_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes){
    *params_bytes = sizeof(biquad_cascade4_coeffs_t);
    *params_bytes = sizeof(biquad_cascade4_params_t);
}

/*******************************************************************************
 * Initializes params, coeffs and states to default values for the requested SR.
 * 
 * @param[in]  params       pointer to the pre-allocated params
 * @param[in]  coeffs       pointer to the pre-allocated coeffs
 * @param[in]  sample_rate  sampling rate
 * 
 ******************************************************************************/
void biquad_cascade4_control_initialize(
    biquad_cascade4_params_t*       params,
    biquad_cascade4_coeffs_t*       coeffs,
    uint32_t                        sample_rate){
    for(int i = 0; i < 4; i++){
        params->cascade4[i].freq = 100.0 + (i * 200);
        params->cascade4[i].gain = 3.0;
        params->cascade4[i].Q = 6.0;
        params->cascade4[i].type = PQE;
        params->cascade4[i].sample_rate = (double)sample_rate;
        params->cascade4[i].Enable = 1;
    }

    biquad_cascade4_update_coeffs((biquad_cascade4_params_t const*)params, coeffs);
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
int32_t biquad_cascade4_set_parameter(
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
void biquad_cascade4_update_coeffs(
    biquad_cascade4_params_t const* params,
    biquad_cascade4_coeffs_t*       coeffs){
    biquad_coeffs_t biquad_coeffs;
    for(uint32_t i = 0; i < 4; i++){
        biquad_update_coeffs((biquad_params_t const*)&params->cascade4[i], &biquad_coeffs);
        for(int y = 0; y < 2; y++){
            coeffs->a1.ch[y + i*2] = biquad_coeffs.a1.ch[y];
            coeffs->a2.ch[y + i*2] = biquad_coeffs.a2.ch[y];
            coeffs->b0.ch[y + i*2] = biquad_coeffs.b0.ch[y];
            coeffs->b1.ch[y + i*2] = biquad_coeffs.b1.ch[y];
            coeffs->b2.ch[y + i*2] = biquad_coeffs.b2.ch[y];
        }
    }
    for(uint32_t i = 0; i < 4; i++){
        for(int y = 0; y < 2; y++){
            printf("coeffs->a1.ch[%d] %f\n", (y + i*2), coeffs->a1.ch[y + i*2]);
            printf("coeffs->a2.ch[%d] %f\n", (y + i*2), coeffs->a2.ch[y + i*2]);
            printf("coeffs->b0.ch[%d] %f\n", (y + i*2), coeffs->b0.ch[y + i*2]);
            printf("coeffs->b1.ch[%d] %f\n", (y + i*2), coeffs->b1.ch[y + i*2]);
            printf("coeffs->b2.ch[%d] %f\n", (y + i*2), coeffs->b2.ch[y + i*2]);
        }
    }
}

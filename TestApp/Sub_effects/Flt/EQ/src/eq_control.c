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

    for(uint32_t y = 0; y < 4; y++){
        params->cascade4[0].cascade4[y].freq = 100.0 + (y * 100);
        params->cascade4[0].cascade4[y].gain = 3.0;
        params->cascade4[0].cascade4[y].Q = 6.0;
        params->cascade4[0].cascade4[y].type = PQE;
        params->cascade4[0].cascade4[y].sample_rate = (double)sample_rate;
        params->cascade4[0].cascade4[y].Enable = 1;
    }
    for(uint32_t y = 0; y < 4; y++){
        params->cascade4[1].cascade4[y].freq = 150.0 + (y * 3000);
        params->cascade4[1].cascade4[y].gain = 3.0;
        params->cascade4[1].cascade4[y].Q = 6.0;
        params->cascade4[1].cascade4[y].type = PQE;
        params->cascade4[1].cascade4[y].sample_rate = (double)sample_rate;
        params->cascade4[1].cascade4[y].Enable = 1;
    }
    for(uint32_t i = 0; i < 2; i++){
        params->cascade2.cascade2[i].freq = 10000 + (i * 1000);
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
    eq_params_t*        params,
    int32_t             id,
    float               value){
    int32_t res = -1;
    id_union_t _id;
    _id.id = id;
    id_union_t cascade_id;
    cascade_id.id = id;
    
    switch (_id.sub_effect)
    {
    case 0:
        res = biquad_cascade4_set_parameter(&params->cascade4[0], cascade_id.id, value);
        break;
    case 1:
        res = biquad_cascade4_set_parameter(&params->cascade4[0], cascade_id.id, value);
        break;
    case 2:
        res = biquad_cascade4_set_parameter(&params->cascade4[0], cascade_id.id, value);
        break;
    case 3:
        res = biquad_cascade4_set_parameter(&params->cascade4[0], cascade_id.id, value);
        break;
    case 4:
        cascade_id.sub_effect  = cascade_id.sub_effect%4;
        res = biquad_cascade4_set_parameter(&params->cascade4[1], cascade_id.id, value);
        break;
    case 5:
        cascade_id.sub_effect = cascade_id.sub_effect % 4;
        res = biquad_cascade4_set_parameter(&params->cascade4[1], cascade_id.id, value);
        break;
    case 6:
        cascade_id.sub_effect = cascade_id.sub_effect % 4;
        res = biquad_cascade4_set_parameter(&params->cascade4[1], cascade_id.id, value);
        break;
    case 7:
        cascade_id.sub_effect = cascade_id.sub_effect % 4;
        res = biquad_cascade4_set_parameter(&params->cascade4[1], cascade_id.id, value);
        break;
    case 8:
        cascade_id.sub_effect = cascade_id.sub_effect % 8;
        res = biquad_cascade2_set_parameter(&params->cascade2, cascade_id.id, value);
        break;
    case 9:
        cascade_id.sub_effect = cascade_id.sub_effect % 8;
        res = biquad_cascade2_set_parameter(&params->cascade2, cascade_id.id, value);
        break;
    default:
        break;
    }
    if(res){
        fprintf(stderr, RED "Error:\t"RESET BOLDWHITE"EQ. Wrong parametr in biquad id %d. Rejected\n"RESET, _id.sub_effect );
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
void eq_update_coeffs(
    eq_params_t const* params,
    eq_coeffs_t*       coeffs){

    for(uint32_t i = 0; i < 2; i++){
        biquad_cascade4_update_coeffs((biquad_cascade4_params_t const*)&params->cascade4[i], &coeffs->cascade4[i]);
    }
    biquad_cascade2_update_coeffs(&params->cascade2, &coeffs->cascade2);
}

#include "compressors.h"
#include "compressors_control.h"

/*******************************************************************************
 * Provides with the rcompressor_4banduired data sizes for parameters and coefficients.
 *   It is caller responsibility to allocate enough memory (bytes) for them.
 * 
 * @param[out] params_bytes   rcompressor_4banduired data size for storing parameters
 * @param[out] coeffs_bytes   rcompressor_4banduired data size for storing coefficients
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
    uint32_t                           sample_rate){
    crossover4b_control_initialize(&params->crossover, &coeffs->crossover, sample_rate);
    params->threshold       [0] = -18.0;
    params->ratio           [0] = 400.0;
    params->tau_attack      [0] = 10.0;
    params->tau_release     [0] = 40.0;
    params->tau_env_attack  [0] = 0.0000000000001;
    params->tau_env_release [0] = 30;
    params->makeUpGain      [0] = 0.0;
    params->enable          [0] = 1;

    params->threshold       [1] = -3.0;
    params->ratio           [1] = 400.0;
    params->tau_attack      [1] = 10.0;
    params->tau_release     [1] = 40.0;
    params->tau_env_attack  [1] = 0.0000000000001;
    params->tau_env_release [1] = 30;
    params->makeUpGain      [1] = 0.0;
    params->enable          [1] = 1;

    params->threshold       [2] = -12.0;
    params->ratio           [2] = 400.0;
    params->tau_attack      [2] = 10.0;
    params->tau_release     [2] = 40.0;
    params->tau_env_attack  [2] = 0.0000000000001;
    params->tau_env_release [2] = 30;
    params->makeUpGain      [2] = 0.0;
    params->enable          [2] = 1;

    params->threshold       [3] = -6.0;
    params->ratio           [3] = 400.0;
    params->tau_attack      [3] = 10.0;
    params->tau_release     [3] = 40.0;
    params->tau_env_attack  [3] = 0.0000000000001;
    params->tau_env_release [3] = 30;
    params->makeUpGain      [3] = 0.0;
    params->enable          [3] = 1;
    
    params->sample_rate = (double)sample_rate;

    compressor_4band_update_coeffs((compressor_4band_params_t const*) params, coeffs);

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
    double a_attack[4], a_release[4], e_attack[4], e_release[4], threshold[4], makeUpGain[4];
    for(uint32_t i = 0; i < 4; i++){
        makeUpGain[i] = pow(10.0, (params->makeUpGain[i]/20.0));
        threshold[i]  = pow(10.0, (params->threshold[i]/20.0));
        a_attack [i]  = pow(M_e, (-(log(9.0)) / (0.001*params->tau_attack[i]*params->sample_rate)));
        a_release[i]  = pow(M_e, (-(log(9.0)) / (0.001*params->tau_release[i]*params->sample_rate)));
        e_attack [i]  = pow(M_e, (-(log(9.0)) / (0.001*params->tau_env_attack[i]*params->sample_rate)));
        e_release[i]  = pow(M_e, (-(log(9.0)) / (0.001*params->tau_env_release[i]*params->sample_rate)));
    }
    for(uint32_t i = 0, c = 0; i < 4; i++){
        for(uint32_t s = 0; s < 2; s++, c++){
            coeffs->e_release   .ch[c] = (mfloat)e_release[i];
            coeffs->e_release_m .ch[c] = (mfloat)(1.0 - e_release[i]);
            coeffs->a_release   .ch[c] = (mfloat)a_release[i];
            coeffs->a_release_m .ch[c] = (mfloat)(1.0 - a_release[i]);
            coeffs->e_attack    .ch[c] = (mfloat)e_attack[i];
            coeffs->e_attack_m  .ch[c] = (mfloat)(1.0 - e_attack[i]);
            coeffs->a_attack    .ch[c] = (mfloat)a_attack[i];
            coeffs->a_attack_m  .ch[c] = (mfloat)(1.0 - a_attack[i]);
            coeffs->threshold   .ch[c] = (mfloat)threshold[i];
            coeffs->ratio       .ch[c] = (mfloat)params->ratio[i];
            coeffs->makeUpGain  .ch[c] = (mfloat)makeUpGain[i];
            coeffs->enable_mask .ch[c] = (params->enable[i]) ? 0.0f : 1.0f;
        }
    }
}
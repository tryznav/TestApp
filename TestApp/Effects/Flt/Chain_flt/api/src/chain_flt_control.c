#include "chain_flt_api.h"

/*******************************************************************************
 * Provides with the required data sizes for parameters and coefficients.
 *   It is caller responsibility to allocate enough memory (bytes) for them.
 * 
 * @param[out] params_bytes   required data size for storing parameters
 * @param[out] coeffs_bytes   required data size for storing coefficients
 * 
 * @return 0 if success, non-zero error code otherwise
 ******************************************************************************/
int32_t chain_flt_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes){
    *params_bytes = sizeof(cross_prm_t) + sizeof(comp_prm_t) + sizeof(eq_prm_t);
    *coeffs_bytes = sizeof(cross_flt_coef_t) + sizeof(comp_flt_coef_t) + sizeof(eq_flt_coef_t);
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
int32_t apf_flt_control_initialize(
    void*       params,
    void*       coeffs,
    uint32_t    sample_rate){

    chain_flt_prm_t *prm = (chain_flt_prm_t *)params;
    chain_flt_coef_t *coef = (chain_flt_coef_t *)coeffs;

    prm->comp_enable = 1;
    prm->cross_enable = 1;
    prm->eq_enable = 1;

    comp_flt_set_prm(&prm->comp, (double)sample_rate);
    cross_flt_set_prm(&prm->cross, (double)sample_rate);
    eq_flt_set_prm(&prm->eq, (double)sample_rate);

    comp_flt_coef(&prm->comp, &coef->comp);
    cross_flt_coef(&prm->cross, &coef->cross);
    eq_flt_coef(&prm->eq,  &coef->eq);

    coef->comp_enable = prm->comp_enable;
    coef->cross_enable =  prm->cross_enable;
    coef->eq_enable = prm->eq_enable;
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
int32_t chain_flt_set_parameter(
    void*       params,
    int32_t     id,
    float       value){

//to work...

}

/*******************************************************************************
 * Calculate coefficients, corresponding to the parameters.
 * 
 * @param[in] params    initialized params
 * @param[in] value     parameter value
 * 
 * @return 0 if success, non-zero error code otherwise
 ******************************************************************************/
int32_t chain_flt_update_coeffs(
    void const* params,
    void*       coeffs){
  
    chain_flt_prm_t *prm = (chain_flt_prm_t *)params;
    chain_flt_coef_t *coef = (chain_flt_coef_t *)coeffs;

    comp_flt_coef(&prm->comp, &coef->comp);
    cross_flt_coef(&prm->cross, &coef->cross);
    eq_flt_coef(&prm->eq,  &coef->eq);

    coef->comp_enable = prm->comp_enable;
    coef->cross_enable =  prm->cross_enable;
    coef->eq_enable = prm->eq_enable;

    coef->comp_enable = prm->comp_enable;
    coef->cross_enable =  prm->cross_enable;
    coef->eq_enable = prm->eq_enable;

    return 0;
}

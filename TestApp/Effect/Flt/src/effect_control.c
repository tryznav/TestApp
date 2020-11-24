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
int32_t effect_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes){
    *params_bytes = sizeof(chain_prm_t);
    *coeffs_bytes = sizeof(chain_coef_t);
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
int32_t effect_control_initialize(
    void*       params,
    void*       coeffs,
    uint32_t    sample_rate){

    chain_prm_t *prm = (chain_prm_t *)params;
    chain_coef_t *coef = (chain_coef_t *)coeffs;

// ______________Set default parameters_______________

    prm->eq1_enable = 1;
    prm->cross_enable = 1;
    // prm->comp_1b_enable = 1;
    // prm->comp_2b_enable = 1;
    // prm->comp_3b_enable = 1;
    // prm->comp_4b_enable = 1;
    // prm->eq2_enable = 1;
    // prm->limiter_enable = 1;

    eq_set_prm_d(&prm->eq1, (double)sample_rate);

    cross_set_prm_d(&prm->cross, (double)sample_rate);

    // comp_flt_set_prm_d(&prm->comp_1b, (double)sample_rate);
    // comp_flt_set_prm_d(&prm->comp_2b, (double)sample_rate);
    // comp_flt_set_prm_d(&prm->comp_3b, (double)sample_rate);
    // comp_flt_set_prm_d(&prm->comp_4b, (double)sample_rate);

    // eq_flt_set_prm_d(&prm->eq2, (double)sample_rate);

    // comp_flt_set_prm_d(&prm->limiter, (double)sample_rate);

// ______________Calculate coefficients, corresponding to the default parameters____________

    effect_update_coeffs(params, coeffs);
    printf("chain_flt_control_initialize\n");
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
int32_t effect_set_parameter(
    void*       params,
    int32_t     id,
    float       value){
    id_union_t _id;
    _id.all = id;
    chain_prm_t *prm = (chain_prm_t *)params;

    switch (_id.id.effect)
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
int32_t effect_update_coeffs(
    void const* params,
    void*       coeffs){
  
    chain_prm_t *prm = (chain_prm_t *)params;
    chain_coef_t *coef = (chain_coef_t *)coeffs;

// ______________Calculate coefficients, corresponding to the parameters_______________

    eq_calculate_coef(&prm->eq1,  &coef->eq1);

    cross_calculate_coef(&prm->cross,  &coef->cross);
    // comp_flt_coef(&prm->comp_1b, &coef->comp_1b);
    // comp_flt_coef(&prm->comp_2b, &coef->comp_2b);
    // comp_flt_coef(&prm->comp_3b, &coef->comp_3b);
    // comp_flt_coef(&prm->comp_4b, &coef->comp_4b);

    // eq_flt_coef(&prm->eq2,  &coef->eq2);

    // comp_flt_coef(&prm->limiter, &coef->limiter);

    coef->eq1_enable =          prm->eq1_enable;
    coef->cross_enable =        prm->cross_enable;
    // coef->comp_1b_enable =      prm->comp_1b_enable; 
    // coef->comp_2b_enable =      prm->comp_2b_enable;
    // coef->comp_3b_enable =      prm->comp_3b_enable;
    // coef->comp_4b_enable =      prm->comp_4b_enable;
    // coef->eq2_enable =          prm->eq2_enable;
    // coef->limiter_enable =      prm->limiter_enable;


    return 0;
}

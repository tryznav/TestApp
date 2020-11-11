#include "apf_api.h"

static int32_t apf_coeff_calc(apf_prm_t *prm, coef_t *coef){
    printf("format %d\n", prm->form);


    switch (prm->order)
    {
    case 1:
        switch (prm->form)
        {
        case 1:
            coef->apf = &apf_flt_1st_DirectI;
            break;
        case 2:
            coef->apf = &apf_flt_1st_DirectII;
            break;
        default:
            fprintf(stderr, RED "Error:\t"RESET BOLDWHITE"Wrong APF form, app has 1-5. Rejected\n"RESET);
            break;
        }
        coef->apf_dbl = apf_dbl_1st_DirectII;
        apf_flt_1st_coef(&(coef->apf_coef), prm->freq,  prm->sample_rate, prm->form);
        break;
    case 2:
        switch (prm->form)
        {
        case 1:
            coef->apf = &apf_flt_2nd_DirectI;
            break;
        case 2:
            coef->apf = &apf_flt_2nd_DirectI_t;
            break;
        case 3:
            coef->apf = &apf_flt_2nd_DirectII;
            break;
        case 4:
            coef->apf = &apf_flt_2nd_DirectII_t;
            break;
        case 5:    //Nested form coef
            coef->apf = &apf_2nd_Lattice;
            break;
        default:
            fprintf(stderr, RED "Error:\t"RESET BOLDWHITE"Wrong APF form, app has 1-5. Rejected\n"RESET);
            break;
        }
        coef->apf_dbl = apf_dbl_2nd_DirectI;
        apf_flt_2nd_coef(&(coef->apf_coef), prm->freq,  prm->sample_rate, prm->form);
        break;
    default:
        fprintf(stderr, RED "Error:\t"RESET BOLDWHITE"Wrong APF form, app has 1-5. Rejected\n"RESET);
        break;
    }
    return 0;
}


/*******************************************************************************
 * Provides with the required data sizes for parameters and coefficients.
 *   It is caller responsibility to allocate enough memory (bytes) for them.
 * 
 * @param[out] params_bytes   required data size for storing parameters
 * @param[out] coeffs_bytes   required data size for storing coefficients
 * 
 * @return 0 if success, non-zero error code otherwise
 ******************************************************************************/
int32_t apf_flt_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes){
    *params_bytes = sizeof(apf_prm_t);
    *coeffs_bytes = sizeof(apf_coef_t);
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
    apf_prm_t *_prm = (apf_prm_t *)params;
    coef_t      *coef = (coef_t  *)coeffs;

    _prm->sample_rate = (double)sample_rate;
    _prm->freq = 100.0;
    _prm->form = 1;
    _prm->order = 2;
    printf("format %d\n", _prm->form);
    apf_coeff_calc(_prm, coef);

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
int32_t apf_flt_set_parameter(
    void*       params,
    int32_t     id,
    float       value){
    apf_prm_t *_prm = (apf_prm_t *)params;

    switch (id)
    {
    case  PRM_SAMPLE_RATE_ID:
        _prm->sample_rate = (double)value;
        return 0;
    case  PRM_APF_CFREQ:
        _prm->freq = (double)value;
        return 0;
    case  PRM_APF_FORM:
        _prm->form = (int32_t)value;
    case  PRM_APF_ORDER:
        _prm->order = (int32_t)value;
        return 0;
    default:
        fprintf(stderr, RED"Error: "BOLDWHITE"Unsupported params. Rejected.\n"RESET);
        return -1;
    }
}

/*******************************************************************************
 * Calculate coefficients, corresponding to the parameters.
 * 
 * @param[in] params    initialized params
 * @param[in] value     parameter value
 * 
 * @return 0 if success, non-zero error code otherwise
 ******************************************************************************/
int32_t apf_flt_update_coeffs(
    void const* params,
    void*       coeffs){
    printf("format %d\n", ((apf_prm_t *)params)->form);
    printf("order %d\n", ((apf_prm_t *)params)->order);

    apf_coeff_calc((apf_prm_t *)params, (coef_t  *)coeffs);
    

    return 0;
}

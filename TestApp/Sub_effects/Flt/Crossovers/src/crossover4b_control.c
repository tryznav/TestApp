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

    params->apf_compens.freq[0] = 14000.0;
    params->apf_compens.freq[1] = 200.0;
    params->apf_compens.sample_rate = (double)sample_rate;

    params->crossover2b_to_4b.freq[0] = 200.0;
    params->crossover2b_to_4b.freq[1] = 14000.0;
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
    crossover4b_params_t*       params,
    int32_t                     id,
    float                       value){
    int32_t res = 0;
    id_union_t _id;
    _id.id = id;

    switch (_id.prm)
    {
    case 0://Freq1
        if(value < 100.0 || value > params->crossover2b.freq){
            fprintf(stderr, RED "Error:\t"RESET BOLDWHITE"Wrong 'Frequency 1' parametr. Rejected\n"RESET);
            return -1;
        }
        params->crossover2b_to_4b.freq[0] = (double)value;
        params->apf_compens.freq[1] = (double)value;
        res = 0;
        break;
    case 1://Freq2
        if(value < params->crossover2b_to_4b.freq[0] || value > params->crossover2b_to_4b.freq[1] ){
            fprintf(stderr, RED "Error:\t"RESET BOLDWHITE"Wrong 'Frequency 2' parametr. Rejected\n"RESET);
            return -1;
        }
        params->crossover2b.freq = (double)value;
        res = 0;
        break;
    case 2://Freq1
        if(value > 15000.0 || value < params->crossover2b.freq){
            fprintf(stderr, RED "Error:\t"RESET BOLDWHITE"Wrong 'Frequency 3' parametr. Rejected\n"RESET);
            return -1;
        }
        params->crossover2b_to_4b.freq[1] = (double)value;
        params->apf_compens.freq[0] = (double)value;
        res = 0;
        break;
    case 255:
            fprintf(stderr, RED "Error:\t"RESET BOLDWHITE"Wrong 'Can not turn off crossover. Rejected\n"RESET);
            return -1;
        break;
    default:
        break;
    }
    if(res){
        fprintf(stderr, RED "Error:\t"RESET BOLDWHITE"Crossover4b. Wrong parametr id %d. Rejected\n"RESET, _id.prm );
    }
    return res;
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

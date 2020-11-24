#include "test_app.h"

int32_t app_efect_init(pross_waw_t *pr, uint32_t sample_rate){
    size_t      params_bytes = 0;
    size_t      coeffs_bytes = 0;
    size_t      states_bytes = 0;
    void*       params = NULL;
    int32_t     Res;

    effect_t*   effect = (effect_t *)malloc(sizeof(effect_t));

    if (effect == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    effect->coeffs = NULL;
    effect->states = NULL;


    if((Res = effect_control_get_sizes(&params_bytes, &coeffs_bytes)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_control_get_sizes()\n"RESET);
        exit(EXIT_FAILURE);
    }

    if((Res = effect_process_get_sizes(&states_bytes)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_update_coeffs\n"RESET);
        // exit(EXIT_FAILURE);
    }

    params = malloc(params_bytes);
    if (params == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    effect->coeffs = malloc(coeffs_bytes);
    if (effect->coeffs == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    effect->states = malloc(states_bytes);
    if (effect->states == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
    }
    if((Res = effect_control_initialize(params, effect->coeffs, sample_rate)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_update_coeffs\n"RESET);
        // exit(EXIT_FAILURE);
    }

    // if((Res = effect->effect_set_parameter(effect->params, PRM_GAIN_dB_ID, effect_task->prm.gain_dB)) != 0){
    //     fprintf(stderr,RED"Error: "BOLDWHITE"effect_set_parameter(PRM_GAIN_dB_ID)\n"RESET);
    //     // exit(EXIT_FAILURE);
    // }

    if((Res = effect_update_coeffs(params, effect->coeffs)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_update_coeffs\n"RESET);
        // exit(EXIT_FAILURE);
    }
    
    if((Res = effect_reset(effect->coeffs, effect->states)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_update_coeffs\n"RESET);
    }

    pr->effect = effect;

    return 0;
}

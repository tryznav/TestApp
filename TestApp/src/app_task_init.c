
#include "test_app.h"

int32_t app_gain_init(pross_waw_t *pross_waw, char *input_f_path, char *output_f_path, float gain_dB){
    size_t params_bytes = 0;
    size_t coeffs_bytes = 0;
    int32_t Res = -1;

    pross_waw->efect = (efect_t *)malloc(sizeof(efect_t));
    if (pross_waw->efect == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    pross_waw->efect->coeffs = NULL;
    pross_waw->efect->params = NULL;
    pross_waw->buff.size_ms = 10;
    pross_waw->dest_f_path = output_f_path;
    pross_waw->src_f_path = input_f_path;

    if((Res = effect_control_get_sizes(&params_bytes, &coeffs_bytes)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_control_get_sizes()\n"RESET);
        exit(EXIT_FAILURE);
    }

    pross_waw->efect->params = malloc(params_bytes);
    if (pross_waw->efect->params == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    pross_waw->efect->coeffs = malloc(coeffs_bytes);
    if (pross_waw->efect->coeffs == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if((Res = effect_set_parameter(pross_waw->efect->params, PRM_GAIN_dB_ID, gain_dB)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_set_parameter()\n"RESET);
        exit(EXIT_FAILURE);
    }

    pross_waw->efect->effect_control_initialize = &effect_control_initialize;
    pross_waw->efect->effect_process            = &effect_process;
    pross_waw->efect->effect_process_get_sizes  = &effect_process_get_sizes;
    pross_waw->efect->effect_reset              = &effect_reset;
    pross_waw->efect->effect_set_parameter      = &effect_set_parameter;
    pross_waw->efect->effect_update_coeffs      = &effect_update_coeffs;

    return 0;
}



int32_t app_file_proc_init(pross_waw_t *pross_waw, app_func_t* task){
    pross_waw->buff.size_ms = 10;
    pross_waw->dest_f_path = task->output_f_path;
    pross_waw->src_f_path = task->input_f_path;
    return 0;
}

int32_t app_effect_init(pross_waw_t *pross_waw, effect_task_t* effect){
    size_t params_bytes = 0;
    size_t coeffs_bytes = 0;
    int32_t Res = -1;

    pross_waw->efect = (efect_t *)malloc(sizeof(efect_t));
    if (pross_waw->efect == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    pross_waw->efect->coeffs = NULL;
    pross_waw->efect->params = NULL;

    if((Res = effect_control_get_sizes(&params_bytes, &coeffs_bytes)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_control_get_sizes()\n"RESET);
        exit(EXIT_FAILURE);
    }

    pross_waw->efect->params = malloc(params_bytes);
    if (pross_waw->efect->params == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    pross_waw->efect->coeffs = malloc(coeffs_bytes);
    if (pross_waw->efect->coeffs == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if((Res = effect_set_parameter(pross_waw->efect->params, PRM_GAIN_dB_ID, task->effect->prm.gain_dB)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_set_parameter(PRM_GAIN_dB_ID)\n"RESET);
        // exit(EXIT_FAILURE);
    }
    if((Res = effect_set_parameter(pross_waw->efect->params, PRM_GAIN_dB_ID, effect->prm.gain_dB)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_set_parameter(PRM_GAIN_dB_ID)\n"RESET);
        // exit(EXIT_FAILURE);
    }

    pross_waw->efect->effect_control_initialize = &effect_control_initialize;
    pross_waw->efect->effect_process            = &effect_process;
    pross_waw->efect->effect_process_get_sizes  = &effect_process_get_sizes;
    pross_waw->efect->effect_reset              = &effect_reset;
    pross_waw->efect->effect_set_parameter      = &effect_set_parameter;
    pross_waw->efect->effect_update_coeffs      = &effect_update_coeffs;

    return 0;
}
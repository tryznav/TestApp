#include "test_app.h"

static int32_t effect_control(pross_waw_t *pr, wav_hdr_t  *hdr, effect_task_t *effect);
static int32_t effect_states_init(pross_waw_t *pr);

int32_t app_efect_init(pross_waw_t *pr, wav_hdr_t  *hdr, effect_task_t *effect){

    pr->efect = (efect_t *)malloc(sizeof(efect_t));
    if (pr->efect == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    pr->efect->coeffs = NULL;
    pr->efect->params = NULL;

    switch (hdr->FmtChunk->audioFormat)
    {
    case PMC:
        switch (effect->effect_type)
        {
        // case    EFFECT_ID_GAIN: 
        //     pr->efect->effect_control_initialize = &effect_control_initialize;
        //     pr->efect->effect_process            = &effect_process;
        //     pr->efect->effect_process_get_sizes  = &effect_process_get_sizes;
        //     pr->efect->effect_reset              = &effect_reset;
        //     pr->efect->effect_set_parameter      = &effect_set_parameter;
        //     pr->efect->effect_update_coeffs      = &effect_update_coeffs;
        //     break;
        case    EFFECT_ID_FIR:
            //controll
            pr->efect->effect_control_get_sizes  = &effect_control_get_sizes;
            pr->efect->effect_control_initialize = &effect_control_initialize;
            pr->efect->effect_set_parameter      = &effect_set_parameter;
            pr->efect->effect_update_coeffs      = &effect_update_coeffs;
            //process
            pr->efect->effect_process            = &effect_process;
            pr->efect->effect_process_get_sizes  = &effect_process_get_sizes;
            pr->efect->effect_reset              = &effect_reset;
        default:
            fprintf(stderr,RED"%d: Error: "BOLDWHITE"Not supported format for this effect.\n"RESET);
            return -1;
            break;
        }
        break;
    case IEEE_754:
        switch (effect->effect_type)
        {
        case    EFFECT_ID_GAIN:
            //controll
            pr->efect->effect_control_get_sizes  = &effect_control_get_sizes;
            pr->efect->effect_control_initialize = &effect_control_initialize;
            pr->efect->effect_set_parameter      = &effect_set_parameter;
            pr->efect->effect_update_coeffs      = &effect_update_coeffs;
            //process
            pr->efect->effect_process            = &effect_process;
            pr->efect->effect_process_get_sizes  = &effect_process_get_sizes;
            pr->efect->effect_reset              = &effect_reset;
            break;
        case    EFFECT_ID_FIR:
            //controll
            pr->efect->effect_control_get_sizes  = &effect_control_get_sizes;
            pr->efect->effect_control_initialize = &effect_control_initialize;
            pr->efect->effect_set_parameter      = &effect_set_parameter;
            pr->efect->effect_update_coeffs      = &effect_update_coeffs;
            //process
            pr->efect->effect_process            = &effect_process;
            pr->efect->effect_process_get_sizes  = &effect_process_get_sizes;
            pr->efect->effect_reset              = &effect_reset;
        default:
            break;
        }
        break;

    default:
        break;
    }

    effect_control(pr, hdr, effect);

    effect_states_init(pr);

    return 0;
}

static int32_t effect_control(pross_waw_t *pr, wav_hdr_t  *hdr, effect_task_t *effect){
    size_t params_bytes = 0;
    size_t coeffs_bytes = 0;
    int32_t Res = -1;

    if((Res = pr->efect->effect_control_get_sizes(&params_bytes, &coeffs_bytes)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_control_get_sizes()\n"RESET);
        exit(EXIT_FAILURE);
    }

    pr->efect->params = malloc(params_bytes);
    if (pr->efect->params == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    pr->efect->coeffs = malloc(coeffs_bytes);
    if (pr->efect->coeffs == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if((Res = pr->efect->effect_control_initialize(pr->efect->params, pr->efect->coeffs, hdr->FmtChunk->sampleRate)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_control_initialize\n"RESET);
        // exit(EXIT_FAILURE);
    }

    if((Res = pr->efect->effect_set_parameter(pr->efect->params, PRM_FREQ_START_ID, effect->prm.cutoff_freq)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_set_parameter(PRM_GAIN_dB_ID)\n"RESET);
        // exit(EXIT_FAILURE);
    }

    if((Res = pr->efect->effect_set_parameter(pr->efect->params, PRM_GAIN_dB_ID, effect->prm.gain_dB)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_set_parameter(PRM_GAIN_dB_ID)\n"RESET);
        // exit(EXIT_FAILURE);
    }
    if((Res = pr->efect->effect_update_coeffs(pr->efect->params, pr->efect->coeffs)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_update_coeffs\n"RESET);
        // exit(EXIT_FAILURE);
    }

    return 0;
}

static int32_t effect_states_init(pross_waw_t *pr){
    size_t states_bytes = 0;
    int32_t Res = -1;

    if((Res = pr->efect->effect_process_get_sizes(&states_bytes)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_update_coeffs\n"RESET);
        // exit(EXIT_FAILURE);
    }

    pr->efect->states = malloc(states_bytes);
    if (pr->efect->states == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if((Res = pr->efect->effect_reset(pr->efect->coeffs, pr->efect->states)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_update_coeffs\n"RESET);
        // exit(EXIT_FAILURE);
    }

    return 0;
}

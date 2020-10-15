#include "test_app.h"

static int32_t effect_control(effect_t *effect, wav_hdr_t  *hdr, effect_task_t *effect_task);
// static int32_t effect_states_init(pross_waw_t *pr);

int32_t app_efect_init(pross_waw_t *pr, wav_hdr_t  *hdr, effect_task_t *effect_task){

    effect_t*    efect = (effect_t *)malloc(sizeof(effect_t));
    if (efect == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    efect->coeffs = NULL;
    efect->params = NULL;
    efect->states = NULL;


    switch (hdr->FmtChunk->audioFormat)
    {
    case PCM:
        switch (effect_task->effect_type)
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
            printf("EFFECT_ID_FIR");
            //controll
            efect->effect_control_get_sizes  = &fir_fxd_control_get_sizes;
            efect->effect_control_initialize = &fir_fxd_control_initialize;
            efect->effect_set_parameter      = &fir_fxd_set_parameter;
            efect->effect_update_coeffs      = &fir_fxd_update_coeffs;
            //process
            efect->effect_process            = &fir_fxd_process;
            efect->effect_process_get_sizes  = &fir_fxd_process_get_sizes;
            efect->effect_reset              = &fir_fxd_reset;
            break;
        case    EFFECT_ID_IIR:
        printf("EFFECT_ID_IIR");
            //controll
            efect->effect_control_get_sizes  = &iir_fxd_control_get_sizes;
            efect->effect_control_initialize = &iir_fxd_control_initialize;
            efect->effect_set_parameter      = &iir_fxd_set_parameter;
            efect->effect_update_coeffs      = &iir_fxd_update_coeffs;
            //process
            efect->effect_process            = &iir_fxd_process;
            efect->effect_process_get_sizes  = &iir_fxd_process_get_sizes;
            efect->effect_reset              = &iir_fxd_reset;
             break;
        default:
            fprintf(stderr,RED" Error: "BOLDWHITE"Not supported format for this effect.\n"RESET);
            return -1;
            break;
        }
        break;
    case IEEE_754:
        switch (effect_task->effect_type)
        {
        case    EFFECT_ID_GAIN:
            //controll
            efect->effect_control_get_sizes  = &effect_control_get_sizes;
            efect->effect_control_initialize = &effect_control_initialize;
            efect->effect_set_parameter      = &effect_set_parameter;
            efect->effect_update_coeffs      = &effect_update_coeffs;
            //process
            efect->effect_process            = &effect_process;
            efect->effect_process_get_sizes  = &effect_process_get_sizes;
            efect->effect_reset              = &effect_reset;
            break;
        case    EFFECT_ID_FIR:
            printf("EFFECT_ID_FIR");
            //controll
            efect->effect_control_get_sizes  = &fir_flt_control_get_sizes;
            efect->effect_control_initialize = &fir_flt_control_initialize;
            efect->effect_set_parameter      = &fir_flt_set_parameter;
            efect->effect_update_coeffs      = &fir_flt_update_coeffs;
            //process
            efect->effect_process            = &fir_flt_process;
            efect->effect_process_get_sizes  = &fir_flt_process_get_sizes;
            efect->effect_reset              = &fir_flt_reset;
            break;
        case    EFFECT_ID_IIR:
        printf("EFFECT_ID_IIR");
            //controll
            efect->effect_control_get_sizes  = &iir_flt_control_get_sizes;
            efect->effect_control_initialize = &iir_flt_control_initialize;
            efect->effect_set_parameter      = &iir_flt_set_parameter;
            efect->effect_update_coeffs      = &iir_flt_update_coeffs;
            //process
            efect->effect_process            = &iir_flt_process;
            efect->effect_process_get_sizes  = &iir_flt_process_get_sizes;
            efect->effect_reset              = &iir_flt_reset;
             break;
        default:
            break;
        }
        break;

    default:
        break;
    }

    effect_control(efect, hdr, effect_task);

    pr->efect = efect;

    return 0;
}

static int32_t effect_control(effect_t *effect, wav_hdr_t  *hdr, effect_task_t *effect_task){
    size_t params_bytes = 0;
    size_t coeffs_bytes = 0;
    size_t states_bytes = 0;
    int32_t Res = -1;

    if((Res = effect->effect_control_get_sizes(&params_bytes, &coeffs_bytes)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_control_get_sizes()\n"RESET);
        exit(EXIT_FAILURE);
    }
    if((Res = effect->effect_process_get_sizes(&states_bytes)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_update_coeffs\n"RESET);
        // exit(EXIT_FAILURE);
    }
    
    effect->params = malloc(params_bytes);
    if (effect->params == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    effect->coeffs = malloc(coeffs_bytes);
    if (effect->coeffs == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
fprintf(stderr,RED"%d: Error000: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
    effect->states = malloc(states_bytes);
    if (effect->states == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
    }

    if((Res = effect->effect_control_initialize(effect->params, effect->coeffs, hdr->FmtChunk->sampleRate)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_control_initialize\n"RESET);
        // exit(EXIT_FAILURE);
    }


    if((Res = effect->effect_set_parameter(effect->params, PRM_FREQ_START_ID, effect_task->prm.cutoff_freq.sweep.start)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_set_parameter(PRM_GAIN_dB_ID)\n"RESET);
        // exit(EXIT_FAILURE);
    }

    if((Res = effect->effect_set_parameter(effect->params, PRM_FREQ_END_ID, effect_task->prm.cutoff_freq.sweep.end)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_set_parameter(PRM_GAIN_dB_ID)\n"RESET);
        // exit(EXIT_FAILURE);
    }

    // if((Res = effect->effect_set_parameter(effect->params, PRM_GAIN_dB_ID, effect_task->prm.gain_dB)) != 0){
    //     fprintf(stderr,RED"Error: "BOLDWHITE"effect_set_parameter(PRM_GAIN_dB_ID)\n"RESET);
    //     // exit(EXIT_FAILURE);
    // }
    fprintf(stderr,RED"%d: Error000: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));

    if((Res = effect->effect_update_coeffs(effect->params, effect->coeffs)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_update_coeffs\n"RESET);
        // exit(EXIT_FAILURE);
    }

    
    if((Res = effect->effect_reset(effect->coeffs, effect->states)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_update_coeffs\n"RESET);
    }

    fprintf(stderr,RED"%d: Error000: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
    

    return 0;
}

static int32_t effect_states_init(effect_t *effect){
    size_t states_bytes = 0;
    int32_t Res = -1;
   
    if((Res = effect->effect_process_get_sizes(&states_bytes)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_update_coeffs\n"RESET);
        // exit(EXIT_FAILURE);
    }
    effect->states = malloc(states_bytes);
    if (effect->states == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
    }

    if((Res = effect->effect_reset(effect->coeffs, effect->states)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_update_coeffs\n"RESET);
    }

    return 0;
}

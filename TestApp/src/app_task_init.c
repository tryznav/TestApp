
#include "test_app.h"

int32_t app_gain_init(pross_waw_t *pross_waw,char *input_f_path, char *output_f_path, float gain_dB){
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

    if((Res = effect_set_parameter(pross_waw->efect->params, GAIN_dB_ID, gain_dB)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_set_parameter()\n"RESET);
        exit(EXIT_FAILURE);
    }

    pross_waw->efect->effect_control_initialize = &effect_control_initialize;
    pross_waw->efect->effect_process = &effect_process;
    pross_waw->efect->effect_process_get_sizes = &effect_process_get_sizes;
    pross_waw->efect->effect_reset = &effect_reset;
    pross_waw->efect->effect_set_parameter = &effect_set_parameter;
    pross_waw->efect->effect_update_coeffs = &effect_update_coeffs;

    return 0;
}

int32_t app_generator_init(pross_waw_t *pross_waw, app_func_t *task){
    int32_t Res = -1;
    gen_sig_t *gen_sig = NULL;

    pross_waw->buff.size_ms = 10;
    pross_waw->dest_f_path = task->output_f_path;
    gen_sig = malloc(sizeof(gen_sig_t));
    if (!gen_sig){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    gen_sig->states = NULL;
    gen_sig->sample_rate = task->sample_rate;
    gen_sig->length_sample = (task->sample_rate/1000) * task->length_ms;

    pross_waw->gen_sig = gen_sig;
    if(gen_sig->length_sample == 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"Very low sample rate.\n"RESET);
        return -1;
    }
    switch (task->signal_id)
    {
    case TSIG_DELTA:
        gen_sig->amplitude_coef = 1;
        gen_sig->params = NULL;
        gen_sig->tsig_gen_sig_st = &tsig_gen_delta_st;
        gen_sig->tsig_sig_init_states = &tsig_delta_init_states;
        Res = 0;
        break;
    case TSIG_STEP:
        gen_sig->amplitude_coef = 1;
        gen_sig->params = NULL;
        gen_sig->tsig_gen_sig_st = &tsig_gen_step_st;
        gen_sig->tsig_sig_init_states = &tsig_step_init_states;
        Res = 0;
        break;
    case TSIG_SQUARE:
        gen_sig->amplitude_coef = 1;
        gen_sig->params = malloc(sizeof(tsig_square_prm_t));
        if (gen_sig->params == NULL){
            fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        ((tsig_square_prm_t *)gen_sig->params)->period_ms = task->period_ms;
        gen_sig->tsig_gen_sig_st = &tsig_gen_square_st;
        gen_sig->tsig_sig_init_states = &tsig_square_init_states;
        Res = 0;
        break;
    case TSIG_WNOISE:
        gen_sig->amplitude_coef = 1;
        gen_sig->params = NULL;
        gen_sig->amplitude_coef = (float)powf(10.0f , (0.05f * task->amplitude.whole_file));
        gen_sig->tsig_gen_sig_st = &tsig_gen_wnoise_st;
        gen_sig->tsig_sig_init_states = &tsig_wnoise_init_states;
        Res = 0;
        break;
    case TSIG_SINE:
        gen_sig->params = malloc(sizeof(tsig_sin_prm_t));
        if (gen_sig->params == NULL){
            fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        gen_sig->amplitude_coef = powf(10.0f , (0.05f * task->amplitude.whole_file));
        ((tsig_sin_prm_t *)gen_sig->params)->freq = task->frequency.whole_file_freq;
        gen_sig->tsig_gen_sig_st = &tsig_gen_sine_st;
        gen_sig->tsig_sig_init_states = &tsig_sin_init_states;
        Res = 0;
        break;
    case TSIG_CHIRP_LINEAR:
        gen_sig->params = malloc(sizeof(tsig_chirp_prm_t));
        if (gen_sig->params == NULL){
            fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        ((tsig_chirp_prm_t *)gen_sig->params)->chirp_type_id = TSIG_CHIRP_LINEAR;
        gen_sig->amplitude_coef = powf(10.0f , (0.05f * task->amplitude.whole_file));
        ((tsig_chirp_prm_t *)gen_sig->params)->freq_start = task->frequency.start_freq;
        ((tsig_chirp_prm_t *)gen_sig->params)->freq_stop = task->frequency.end_freq;
        gen_sig->tsig_gen_sig_st = &tsig_gen_chirp_st;
        gen_sig->tsig_sig_init_states = &tsig_chirp_init_states;
        Res = 0;
        break;
    case TSIG_CHIRP_LOGARITM:
        gen_sig->amplitude_coef = 1;
        gen_sig->params = malloc(sizeof(tsig_chirp_prm_t));
        if (gen_sig->params == NULL){
            fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        ((tsig_chirp_prm_t *)gen_sig->params)->chirp_type_id = TSIG_CHIRP_LOGARITM;
         gen_sig->amplitude_coef = (float)powf(10.0f , (0.05f * task->amplitude.whole_file));
        ((tsig_chirp_prm_t *)gen_sig->params)->freq_start = task->frequency.start_freq;
        ((tsig_chirp_prm_t *)gen_sig->params)->freq_stop = task->frequency.end_freq;
        gen_sig->tsig_gen_sig_st = &tsig_gen_chirp_st;
        gen_sig->tsig_sig_init_states = &tsig_chirp_init_states;
        Res = 0;
        break;
    case TSIG_LEVEL_SWEEP:
        gen_sig->amplitude_coef = 1;
        gen_sig->params = malloc(sizeof(tsig_lsw_sin_prm_t));
        if (gen_sig->params == NULL){
            fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        ((tsig_lsw_sin_prm_t *)gen_sig->params)->freq = (float)(task->frequency.whole_file_freq);
        ((tsig_lsw_sin_prm_t *)gen_sig->params)->amp_start_dB = task->amplitude.start_amp_dB;
        ((tsig_lsw_sin_prm_t *)gen_sig->params)->amp_stop_dB = task->amplitude.end_amp_db;
        gen_sig->tsig_gen_sig_st = &tsig_gen_lsw_sin_st;
        gen_sig->tsig_sig_init_states = &tsig_lsw_sin_init_states;
        Res = 0;
        break;
    
    default:
        fprintf(stderr,RED"Error: "BOLDWHITE"Unsupported signal ID.\n"RESET);
        Res = -1;
        break;
    }
    return Res;
}

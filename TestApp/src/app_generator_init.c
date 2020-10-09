
#include "test_app.h"

int32_t app_generator_init(pross_waw_t *pross_waw, tgen_t   *generator, uint16_t audioFormat){
    int32_t Res = -1;
    gen_sig_t *gen_sig = NULL;
    gen_sig = malloc(sizeof(gen_sig_t));
    if (!gen_sig){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    gen_sig->states = NULL;
    gen_sig->sample_rate = generator->sample_rate;
    gen_sig->length_sample = (generator->sample_rate/1000) * generator->length_ms;

    pross_waw->gen_sig = gen_sig;
    if(gen_sig->length_sample == 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"Very low sample rate.\n"RESET);
        free(gen_sig);
        return -1;
    }
    switch (generator->signal_id)
    {
    case TSIG_DELTA:
        gen_sig->params = NULL;
        gen_sig->tsig_gen_sig_st        = &tsig_gen_delta_st;
        gen_sig->tsig_sig_init_states   = &tsig_delta_init_states;
        Res = 0;
        break;
    case TSIG_STEP:
        gen_sig->params = NULL;
        gen_sig->tsig_gen_sig_st        = &tsig_gen_step_st;
        gen_sig->tsig_sig_init_states   = &tsig_step_init_states;
        Res = 0;
        break;
    case TSIG_SQUARE:
        gen_sig->params = malloc(sizeof(tsig_square_prm_t));
        if (gen_sig->params == NULL){
            fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        ((tsig_square_prm_t *)gen_sig->params)->period_ms = generator->period_ms;
        gen_sig->tsig_gen_sig_st        = &tsig_gen_square_st;
        gen_sig->tsig_sig_init_states   = &tsig_square_init_states;
        Res = 0;
        break;
    case TSIG_WNOISE:
        gen_sig->params = malloc(sizeof(tsig_wnoise_prm_t));
        if (gen_sig->params == NULL){
            fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        ((tsig_wnoise_prm_t *)gen_sig->params)->amp_dB = generator->amp_dB.whole_file; // = (float)powf(10.0f , (0.05f * task->amplitude.whole_file));
        gen_sig->tsig_gen_sig_st        = &tsig_gen_wnoise_st;
        gen_sig->tsig_sig_init_states   = &tsig_wnoise_init_states;
        Res = 0;
        break;
    case TSIG_SINE:
        gen_sig->params = malloc(sizeof(tsig_sin_prm_t));
        if (gen_sig->params == NULL){
            fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        ((tsig_sin_prm_t *)gen_sig->params)->amp_dB = generator->amp_dB.whole_file;
        ((tsig_sin_prm_t *)gen_sig->params)->freq = generator->freq_Hz.whole_file;
        gen_sig->tsig_gen_sig_st        = &tsig_gen_sine_st;
        gen_sig->tsig_sig_init_states   = &tsig_sin_init_states;
        Res = 0;
        break;
    case TSIG_CHIRP_LINEAR:
        gen_sig->params = malloc(sizeof(tsig_chirp_prm_t));
        if (gen_sig->params == NULL){
            fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        ((tsig_chirp_prm_t *)gen_sig->params)->chirp_type_id = TSIG_CHIRP_LINEAR;
        ((tsig_chirp_prm_t *)gen_sig->params)->amp_dB = generator->amp_dB.whole_file;
        ((tsig_chirp_prm_t *)gen_sig->params)->freq_start = generator->freq_Hz.sweep.start;
        ((tsig_chirp_prm_t *)gen_sig->params)->freq_stop = generator->freq_Hz.sweep.end;
        gen_sig->tsig_gen_sig_st        = &tsig_gen_chirp_st;
        gen_sig->tsig_sig_init_states   = &tsig_chirp_init_states;
        Res = 0;
        break;
    case TSIG_CHIRP_LOGARITM:
        gen_sig->params = malloc(sizeof(tsig_chirp_prm_t));
        if (gen_sig->params == NULL){
            fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        ((tsig_chirp_prm_t *)gen_sig->params)->chirp_type_id = TSIG_CHIRP_LOGARITM;
        ((tsig_chirp_prm_t *)gen_sig->params)->amp_dB = generator->amp_dB.whole_file;
        ((tsig_chirp_prm_t *)gen_sig->params)->freq_start = generator->freq_Hz.sweep.start;
        ((tsig_chirp_prm_t *)gen_sig->params)->freq_stop = generator->freq_Hz.sweep.end;
        gen_sig->tsig_gen_sig_st        = &tsig_gen_chirp_st;
        gen_sig->tsig_sig_init_states   = &tsig_chirp_init_states;
        Res = 0;
        break;
    case TSIG_LEVEL_SWEEP:
        gen_sig->params = malloc(sizeof(tsig_lsw_sin_prm_t));
        if (gen_sig->params == NULL){
            fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        ((tsig_lsw_sin_prm_t *)gen_sig->params)->freq = generator->freq_Hz.whole_file;
        ((tsig_lsw_sin_prm_t *)gen_sig->params)->amp_start_dB = generator->amp_dB.sweep.start;
        ((tsig_lsw_sin_prm_t *)gen_sig->params)->amp_stop_dB = generator->amp_dB.sweep.end;
        gen_sig->tsig_gen_sig_st        = &tsig_gen_lsw_sin_st;
        gen_sig->tsig_sig_init_states   = &tsig_lsw_sin_init_states;
        Res = 0;
        break;
    
    default:
        fprintf(stderr,RED"Error: "BOLDWHITE"Unsupported signal ID.\n"RESET);
        Res = -1;
        break;
    }

    gen_sig->states = gen_sig->tsig_sig_init_states(gen_sig->sample_rate, gen_sig->length_sample,  gen_sig->params, audioFormat);
    if(gen_sig->states == NULL){
        fprintf(stderr,RED"Error: "BOLDWHITE"tsig_sig_init_states\n"RESET);
        return -1;
    }

    return Res;
}
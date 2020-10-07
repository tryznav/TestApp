#include "test_sig_gen.h"

void *tsig_square_init_states (uint32_t sample_rate, uint32_t length_sample, void const *params, uint16_t audioFormat){
    tsig_square_stat_t *states = NULL;
    tsig_square_prm_t * _prm = (tsig_square_prm_t *) params;
    if(_prm == NULL){
        fprintf(stderr,RED" Error: "BOLDWHITE"NULL pointer params.Rejected.\n"RESET);
        return NULL;
    }

    states = malloc(sizeof(tsig_square_stat_t));
    if(states == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    states->audioFormat = audioFormat;
    states->amplitude_coef = 1;

    states->sample_increment = 0;
    states->sign_value = 1;
    states->half_period_sample = (sample_rate/2000) * _prm->period_ms;

    return states;
}

int32_t tsig_gen_square_st (uint32_t length_sample, void* states, void *audio){
    chanels_t           *_audio = (chanels_t *)audio;
    tsig_square_stat_t  *_st    = (tsig_square_stat_t *)states; 
    int                 n       = 0;

    if((n = check_gen(length_sample, states, audio)) != 0){
        return n;
    }

    for (uint32_t i = 0; i < length_sample; i++){
        _audio[i].Left = _st->sign_value * _st->amplitude_coef;
        _audio[i].Right = _audio[i].Left;

        _st->sample_increment++;
        if(_st->sample_increment == _st->half_period_sample){
            _st->sample_increment = 0;
            _st->sign_value *= -1;
        }
    }
    
    if(_st->audioFormat == PMC){
        IEEE_754_to_PMC(audio, length_sample);
    }

    return 0;
}

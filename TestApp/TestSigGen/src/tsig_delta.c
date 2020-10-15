#include "test_sig_gen.h"

void *tsig_delta_init_states (uint32_t sample_rate, uint32_t length_sample, void const *params, uint16_t audioFormat){
    sample_rate = 0;
    tsig_delta_stat_t *states = NULL;

    states = malloc(sizeof(tsig_delta_stat_t));
    if(states == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    states->audioFormat = audioFormat;
    states->amplitude_coef = 1;
    states->ex = 0;
    return states;

}

int32_t tsig_gen_delta_st(uint32_t length_sample, void* states, void *audio){
    chanels_t           *_audio = (chanels_t *)audio;
    tsig_delta_stat_t   *_st    = (tsig_delta_stat_t *)states; 
    int                 n       = 0;

    if( (n = check_gen(length_sample, states, audio)) != 0){
        return n;
    }

    for (uint32_t i = 0; i < length_sample; i++){
        _audio[i].Right = 0.0f;
        _audio[i].Left = 0.0f;
    }
        
    if(!_st->ex){
        _audio[0].Left = 1.0f * _st->amplitude_coef;
        _audio[0].Right = _audio[0].Left;
        _st->ex = 1;
    }

    if(_st->audioFormat == PCM){
        IEEE_754_to_PCM(audio, length_sample);
    }

    return 0;
}

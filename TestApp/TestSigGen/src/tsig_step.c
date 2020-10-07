
#include "test_sig_gen.h"

void *tsig_step_init_states (uint32_t sample_rate, uint32_t length_sample, void const *params, uint16_t audioFormat){
    tsig_step_stat_t * states = NULL;
    states = malloc(sizeof(tsig_step_stat_t));
    if(states == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    states->audioFormat = audioFormat;
    states->amplitude_coef = 1;
    return states;

}

int32_t tsig_gen_step_st(uint32_t length_sample, void* states, void *audio){
    chanels_t           *_audio = (chanels_t *)audio;
    tsig_step_stat_t    *_st    = (tsig_step_stat_t *)states; 
    int                 n       = 0;

    if((n = check_gen(length_sample, states, audio)) != 0){
        return n;
    }

    for (uint32_t i = 0; i < length_sample; i++){
        _audio[i].Left = 1.0f * _st->amplitude_coef;
        _audio[i].Right = ((chanels_t *)audio)[i].Left;
    }

    if(_st->audioFormat == PMC){
        IEEE_754_to_PMC(audio, length_sample);
    }

    return n;
}


#include "test_sig_gen.h"
#include<time.h>
#include <stdio.h> 
#include <stdlib.h> 

void *tsig_wnoise_init_states (uint32_t sample_rate, uint32_t length_sample, void const *params, uint16_t audioFormat){
    tsig_wnoise_stat_t*         states   = NULL;
    tsig_wnoise_prm_t const*    _prm      =(tsig_wnoise_prm_t const *)params;

    states = malloc(sizeof(tsig_wnoise_stat_t));
    if(states == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    states->audioFormat = audioFormat;
    states->amplitude_coef = powf(10.0f , (0.05f * _prm->amp_dB));

    return states;
}


int32_t tsig_gen_wnoise_st(uint32_t length_sample, void* states, void *audio){
    chanels_t           *_audio = (chanels_t *)audio;
    tsig_wnoise_stat_t  *_st    = (tsig_wnoise_stat_t *)states; 
    int                 n       = 0;

    if((n = check_gen(length_sample, states, audio)) != 0){
        return n;
    }

    for (uint32_t i = 0; i < length_sample ; i++){
        _audio[i].Left =((float)rand()/(float)(RAND_MAX)*2.0f - 1.0f) * _st->amplitude_coef;
        _audio[i].Right = ((chanels_t *)audio)[i].Left;
    }

    if(_st->audioFormat == PCM){
        IEEE_754_to_PCM(audio, length_sample);
    }

    return 0;
}

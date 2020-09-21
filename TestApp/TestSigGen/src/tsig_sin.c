#include "test_sig_gen.h"

void *tsig_sin_init_states (uint32_t sample_rate, uint32_t length_sample, void* states, void const *params){
    sample_rate = 0;
    states = malloc(sizeof(tsig_sin_stat_t));
    if(states == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    ((tsig_sin_stat_t *)(states))->sample_increment = 0;
    ((tsig_sin_stat_t *)(states))->sample_in_period = sample_rate / ((tsig_sin_prm_t *)params)->freq;    
    if(((tsig_sin_stat_t *)(states))->sample_in_period < 2){
        fprintf(stderr,RED"Error: Nyquist frequency."BOLDWHITE"The Nyquist frequency is half of the sampling rate of a discrete signal processing system.\n"RESET);
        free(states);
        return NULL;
    }
    return states;

}

int32_t tsig_gen_sin_st(const uint32_t sample_rate, uint32_t length_sample, float amplitude_coef, const void *params, void* states, void *audio){

    for (uint32_t i = 0; i < length_sample; i++){
        ((chanels_t *)audio)[i].Left = (float)sin((double)((tsig_sin_prm_t *)params)->freq * (2.0 * M_PI) * (double)(((tsig_sin_stat_t *)states)->sample_increment) / (double)sample_rate) * amplitude_coef;
        ((chanels_t *)audio)[i].Right = ((chanels_t *)audio)[i].Left;
        ((tsig_sin_stat_t *)states)->sample_increment++;
        if(((tsig_sin_stat_t *)states)->sample_increment == ((tsig_sin_stat_t *)(states))->sample_in_period){
            ((tsig_sin_stat_t *)states)->sample_increment = 0;
        }
    }

    return 0;
}

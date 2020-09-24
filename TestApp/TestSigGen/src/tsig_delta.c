#include "test_sig_gen.h"

void *tsig_delta_init_states (uint32_t sample_rate, uint32_t length_sample, void const *params){
    sample_rate = 0;
    tsig_delta_stat_t *states = NULL;

    states = malloc(sizeof(tsig_delta_stat_t));
    if(states == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    ((tsig_delta_stat_t *)(states))->ex = 0;
    return states;

}

int32_t tsig_gen_delta_st(uint32_t sample_rate, uint32_t length_sample, float amplitude_coef, void const *params, void* states, void *audio){
    if(states == NULL){
        fprintf(stderr,RED" Error: "BOLDWHITE"NULL pointer states.Rejected.\n"RESET);
        return -1;
    }
    for (uint32_t i = 0; i < length_sample; i++){
        ((chanels_t *)audio)[i].Right = 0.0f;
        ((chanels_t *)audio)[i].Left = 0.0f;
    }
        
    if(!((tsig_delta_stat_t *)(states))->ex){
        ((chanels_t *)audio)[0].Left = 1.0f * amplitude_coef;
        ((chanels_t *)audio)[0].Right = ((chanels_t *)audio)[0].Left;
        ((tsig_delta_stat_t *)(states))->ex = 1;
    }
    return 0;
}

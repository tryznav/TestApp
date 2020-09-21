
#include "test_sig_gen.h"

void *tsig_step_init_states(uint32_t sample_rate, uint32_t length_sample, void* states, void *params){
    sample_rate = 0;
    states = malloc(1);
    if(states == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return states;

}


int32_t tsig_wnoise_st(uint32_t sample_rate, uint32_t length_sample, float amplitude_coef, void const *params, void* states, void *audio){
    sample_rate = 0;

    for (uint32_t i = 0; i < length_sample; i++){
        ((chanels_t *)audio)[i].Left = ((float)rand()/(float)(RAND_MAX) * amplitude_coef);
        ((chanels_t *)audio)[i].Right = ((chanels_t *)audio)[i].Left;
    }

    return 0;
}

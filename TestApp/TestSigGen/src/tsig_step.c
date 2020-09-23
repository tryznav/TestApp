
#include "test_sig_gen.h"

void *tsig_step_init_states (uint32_t sample_rate, uint32_t length_sample, void const *params){
    sample_rate = 0;
    void * states = NULL;
    states = malloc(1);
    if(states == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return states;

}

int32_t tsig_gen_step_st(uint32_t sample_rate, uint32_t length_sample, float amplitude_coef, void const *params, void* states, void *audio){

    for (uint32_t i = 0; i < length_sample; i++){
        ((chanels_t *)audio)[i].Left = 1.0f * amplitude_coef;
        ((chanels_t *)audio)[i].Right = ((chanels_t *)audio)[i].Left;
    }
    return 0;
}

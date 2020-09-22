#include "test_sig_gen.h"

void *tsig_square_init_states (uint32_t sample_rate, uint32_t length_sample, void const *params){
    tsig_square_stat_t *states = NULL;
    sample_rate = 0;
    
    states = malloc(sizeof(tsig_square_stat_t));
    if(states == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    states->sample_increment = 0;
    states->sign_value = 1;
    states->half_period_sample = (sample_rate/1000) * (((tsig_square_prm_t *)params)->period_ms);
    return states;
}

int32_t tsig_gen_square_st(uint32_t sample_rate, uint32_t length_sample, float amplitude_coef, void const *params, void* states, void *audio){
    sample_rate = 0;

    for (uint32_t i = 0; i < length_sample; i++){
        ((chanels_t *)audio)[i].Left = ((tsig_square_stat_t *)(states))->sign_value * amplitude_coef;
        ((chanels_t *)audio)[i].Right = ((chanels_t *)audio)[i].Left;
        ((tsig_square_stat_t *)(states))->sample_increment++;
        if(((tsig_square_stat_t *)(states))->sample_increment == ((tsig_square_stat_t *)(states))->half_period_sample){
            ((tsig_square_stat_t *)(states))->sample_increment = 0;
            ((tsig_square_stat_t *)(states))->sign_value *= -1;
        }
    }

    return 0;
}

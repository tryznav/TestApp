#include "test_sig_gen.h"
void *tsig_square_init_states(uint32_t sample_rate, uint32_t length_sample, void* states){
    sample_rate = 0;
    states = malloc(sizeof(tsig_square_stat_t));
    if(states == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    ((tsig_square_stat_t *)(states))->sample_increment = 0;
    ((tsig_square_stat_t *)(states))->sign_value = 1;
    return states;

}

float *tsig_square_st(uint32_t sample_rate, uint32_t length_sample, void *params, void* states, void *audio){
    sample_rate = 0;
    uint16_t half_period_sample = (sample_rate/1000) * (((tsig_square_prm_t *)params)->period_ms);

    for (uint32_t i = 0; i < length_sample; i++){
        ((chanels_t *)audio)[i].Right = ((tsig_square_stat_t *)(states))->sign_value;
        ((chanels_t *)audio)[i].Left = 0.0f;
        ((tsig_square_stat_t *)(states))->sample_increment++;
        if(((tsig_square_stat_t *)(states))->sample_increment == half_period_sample){
            ((tsig_square_stat_t *)(states))->sample_increment = 0;
            ((tsig_square_stat_t *)(states))->sample_increment *= -1;
        }
    }

}

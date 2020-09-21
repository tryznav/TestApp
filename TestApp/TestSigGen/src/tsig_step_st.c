
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

float *tsig_wnoise_st(uint32_t sample_rate, uint32_t length_sample, void *params, void* states, void *audio){
    sample_rate = 0;
    float coef = powf(10.0f , (0.05f * ((tsig_wnoise_prm_t *)params)->amplitude));

    for (uint32_t i = 0; i < length_sample; i++){
        ((chanels_t *)audio)[i].Right = ((float)rand()/(float)(RAND_MAX) * coef);
        ((chanels_t *)audio)[i].Left = 0.0f;
    }

}

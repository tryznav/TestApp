#include "test_sig_gen.h"

void *tsig_chirp_states(uint32_t sample_rate, uint32_t length_sample, void* states, void const *params){
    sample_rate = 0;
    states = malloc(sizeof(tsig_chirp_stat_t));
    if(states == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    switch (((tsig_chirp_prm_t *)(params))->chirp_type_id)
    {
    case CHIRP_LINEAR:
        ((tsig_chirp_stat_t *)(states))->freq_increment_num = (float)(((tsig_chirp_prm_t *)(params))->freq_stop) - (float)(((tsig_chirp_prm_t *)(params))->freq_start);
        ((tsig_chirp_stat_t *)(states))->freq_increment_num /= (float)length_sample;
        break;
    case CHIRP_LOGARITM:

        break;
    default:
        fprintf(stderr,RED"Error: "BOLDWHITE"Wrong Chirp type.\n"RESET );
        free(states);
        return -1;
    }
    ((tsig_chirp_stat_t *)(states))->freq_increment = (float)(((tsig_chirp_prm_t *)(params))->freq_start);
    ((tsig_chirp_stat_t *)(states))->sample_increment = 0;

    return states;
}

int32_t tsig_chirp_st(uint32_t sample_rate, uint32_t length_sample, float amplitude_coef, void const *params, void* states, void *audio){

    for (uint32_t i = 0; i < length_sample; i++){
        ((chanels_t *)audio)[i].Left = (float)sin(((tsig_chirp_stat_t *)(states))->freq_increment * (2.0 * M_PI) * (double)(((tsig_sin_stat_t *)states)->sample_increment) / (double)sample_rate) * amplitude_coef;
        ((chanels_t *)audio)[i].Right = ((chanels_t *)audio)[i].Left;
        ((tsig_sin_stat_t *)states)->sample_increment++;
        ((tsig_chirp_stat_t *)(states))->freq_increment += ((tsig_chirp_stat_t *)(states))->freq_increment_num;
    }

    return 0;
}

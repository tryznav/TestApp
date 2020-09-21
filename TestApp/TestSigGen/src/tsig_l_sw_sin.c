#include "test_sig_gen.h"

void *tsig_l_sw_sin_init_states(uint32_t sample_rate, uint32_t length_sample, void* states, void const *params){
    sample_rate = 0;
    float  amp_coef_start = powf(10.0f , (0.05f * (float)((tsig_l_sw_sin_prm_t *)params)->amp_start_dB));
    float  amp_coef_stop = powf(10.0f , (0.05f * (float)((tsig_l_sw_sin_prm_t *)params)->amp_stop_dB));

    states = malloc(sizeof(tsig_l_sw_sin_stat_t));
    if(states == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    ((tsig_l_sw_sin_stat_t *)(states))->amp_increment_num = (amp_coef_stop - amp_coef_start)/(float)(length_sample);
    ((tsig_l_sw_sin_stat_t *)(states))->amp_increment = amp_coef_start;
    ((tsig_l_sw_sin_stat_t *)(states))->sin_coef = (double)((tsig_l_sw_sin_prm_t *)params)->freq * (2.0 * M_PI) / (double)sample_rate;
    return states;
}

int32_t tsig_l_sw_sin_st(uint32_t sample_rate, uint32_t length_sample, float amplitude_coef, void const *params, void* states, void *audio){
    amplitude_coef = 0;
    sample_rate = 0;

    for (uint32_t i = 0; i < length_sample; i++){
        ((chanels_t *)audio)[i].Left = (float)sin((double)(((tsig_l_sw_sin_stat_t *)states)->sin_coef) * (double)(((tsig_l_sw_sin_stat_t *)states)->sample_increment)) * ((tsig_l_sw_sin_stat_t *)(states))->amp_increment;
        ((chanels_t *)audio)[i].Right = ((chanels_t *)audio)[i].Left;
        ((tsig_l_sw_sin_stat_t *)(states))->amp_increment += ((tsig_l_sw_sin_stat_t *)(states))->amp_increment_num;
        ((tsig_l_sw_sin_stat_t *)states)->sample_increment++;
    }

    return 0;
}

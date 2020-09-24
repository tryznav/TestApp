#include "test_sig_gen.h"

void *tsig_lsw_sin_init_states (uint32_t sample_rate, uint32_t length_sample, void const *params){
    tsig_lsw_sin_stat_t     *states = NULL;
    tsig_lsw_sin_prm_t      *_params = (tsig_lsw_sin_prm_t *)params;
    if(_params == NULL){
        fprintf(stderr,RED" Error: "BOLDWHITE"NULL pointer states.Rejected.\n"RESET);
        return NULL;
    }
    float  amp_coef_start = powf(10.0f , (0.05f * _params->amp_start_dB));
    float  amp_coef_stop = powf(10.0f , (0.05f * _params->amp_stop_dB));

    states = (tsig_lsw_sin_stat_t *)malloc(sizeof(tsig_lsw_sin_stat_t));
    if(states == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    states->amp_increment_num = (amp_coef_stop - amp_coef_start)/(float)(length_sample);
    states->amp_increment = amp_coef_start;
    states->sin_coef = (double)(_params->freq * (2.0 * M_PI)) / (double)sample_rate;
    return states;
}

int32_t tsig_gen_lsw_sin_st(uint32_t sample_rate, uint32_t length_sample, float amplitude_coef, void const *params, void* states, void *audio){
    tsig_lsw_sin_stat_t * _states = (tsig_lsw_sin_stat_t *)states;

    if(_states == NULL){
        fprintf(stderr,RED" Error: "BOLDWHITE"NULL pointer states.Rejected.\n"RESET);
        return -1;
    }

    for (uint32_t i = 0; i < length_sample; i++){
        ((chanels_t *)audio)[i].Left = (float)sin(_states->sin_coef * (double)(_states->sample_increment)) * (_states->amp_increment);
        ((chanels_t *)audio)[i].Right = ((chanels_t *)audio)[i].Left;
        _states->amp_increment = _states->amp_increment + _states->amp_increment_num;
        _states->sample_increment++;
    }


    return 0;
}

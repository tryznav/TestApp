#include "test_sig_gen.h"

void *tsig_chirp_init_states (uint32_t sample_rate, uint32_t length_sample, void const *params){
    sample_rate = 0;
    tsig_chirp_stat_t   *_states = NULL;;
    tsig_chirp_prm_t    *_params  = (tsig_chirp_prm_t  *)params;

    _states = malloc(sizeof(tsig_chirp_stat_t));
    if(_states == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    switch (((tsig_chirp_prm_t *)(params))->chirp_type_id)
    {
    case TSIG_CHIRP_LINEAR:
        _states->freq_increment_num = (float)(_params->freq_stop) - (float)(_params->freq_start);
        _states->freq_increment_num /= (float)length_sample;
        break;
    case TSIG_CHIRP_LOGARITM:

        break;
    default:
        fprintf(stderr,RED"Error: "BOLDWHITE"Wrong Chirp type.\n"RESET);
        free(_states);
        return NULL;
    }
    _states->freq_increment = (float)_params->freq_start;
    _states->sample_increment = 0;

    return _states;
}

int32_t tsig_gen_chirp_st(uint32_t sample_rate, uint32_t length_sample, float amplitude_coef, void const *params, void* states, void *audio){
    tsig_chirp_stat_t *_states = (tsig_chirp_stat_t *)states;
    chanels_t  *_audio = (chanels_t *)audio;

    for (uint32_t i = 0; i < length_sample; i++){
        _audio[i].Left = (float)sin((_states->freq_increment * (2.0 * M_PI) * (double)_states->sample_increment) / (double)sample_rate) * amplitude_coef;
        _audio[i].Right = ((chanels_t *)audio)[i].Left;
        _states->sample_increment++;
        _states->freq_increment += _states->freq_increment_num;
    }

    return 0;
}

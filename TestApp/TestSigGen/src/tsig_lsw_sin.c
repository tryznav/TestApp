#include "test_sig_gen.h"

void *tsig_lsw_sin_init_states (uint32_t sample_rate, uint32_t length_sample, void const *params, uint16_t audioFormat){
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

int32_t tsig_gen_lsw_sin_st(uint32_t length_sample, void* states, void *audio){
    chanels_t           *_audio = (chanels_t *)audio;
    tsig_lsw_sin_stat_t *_st    = (tsig_lsw_sin_stat_t *)states; 
    int                 n       = 0;

    if( (n = check_gen(length_sample, states, audio)) != 0){
        return n;
    }

    for (uint32_t i = 0; i < length_sample; i++){
        _audio[i].Left = sinf((float)_st->sin_coef * (float)(_st->sample_increment)) * (_st->amp_increment);
        _audio[i].Right = _audio[i].Left;
        _st->amp_increment = _st->amp_increment + _st->amp_increment_num;
        _st->sample_increment++;
    }

    if(_st->audioFormat == PMC){
        IEEE_754_to_PMC(audio, length_sample);
    }

    return n;
}

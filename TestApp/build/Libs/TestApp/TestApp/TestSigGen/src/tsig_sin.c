#include "test_sig_gen.h"

void *tsig_sin_init_states (uint32_t sample_rate, uint32_t length_sample, void const *params, uint16_t audioFormat){
    
    tsig_sin_prm_t const *  _prm   = (tsig_sin_prm_t const *)params; 
    tsig_sin_stat_t *       states = NULL;
    
    if(!params){
        fprintf(stderr,RED" Error: "BOLDWHITE"NULL pointer states.Rejected.\n"RESET);
        return NULL;
    }

    states = malloc(sizeof(tsig_sin_stat_t));

    if(states == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    states->audioFormat = audioFormat;

    states->sample_increment = 0;
    states->sample_in_period = sample_rate / (uint32_t)_prm->freq; 
    if(states->sample_in_period < 2){
        fprintf(stderr,RED"Error: Nyquist frequency."BOLDWHITE"The Nyquist frequency is half of the sampling rate of a discrete signal processing system.\n"RESET);
        free(states);
        return NULL;
    }
    states->sample_rate = (float)sample_rate;
    states->freq        = _prm->freq;
    states->amplitude_coef = powf(10.0f , (0.05f * _prm->amp_dB));
    return states;

}

int32_t tsig_gen_sine_st (uint32_t length_sample, void* states, void *audio){
    chanels_t           *_audio = (chanels_t *)audio;
    tsig_sin_stat_t     *_st    = (tsig_sin_stat_t *)states; 
    int                 n       = 0;

    if((n = check_gen(length_sample, states, audio)) != 0){
        return n;
    }

    for (uint32_t i = 0; i < length_sample; i++){
        _audio[i].Left = sinf(_st->freq * (float)(2.0 * M_PI) * (float)_st->sample_increment / _st->sample_rate) * _st->amplitude_coef;
        _audio[i].Right = _audio[i].Left;
        _st->sample_increment++;
        if(_st->sample_increment == _st->sample_in_period){
            _st->sample_increment = 0;
        }
    }

    if(_st->audioFormat == PCM){
        IEEE_754_to_PCM(audio, length_sample);
    }

    return n;
}

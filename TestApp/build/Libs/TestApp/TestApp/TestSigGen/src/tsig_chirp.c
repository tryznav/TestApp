#include "test_sig_gen.h"

void *tsig_chirp_init_states (uint32_t sample_rate, uint32_t length_sample, void const *params, uint16_t audioFormat){
    tsig_chirp_stat_t   *_states = NULL;
    tsig_chirp_prm_t    *_params  = (tsig_chirp_prm_t  *)params;

    _states = malloc(sizeof(tsig_chirp_stat_t));
    if(_states == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    switch (_params->chirp_type_id)
    {
    case TSIG_CHIRP_LINEAR:
        _states->freq_diff = _params->freq_stop - _params->freq_start;
        _states->freq_pow = 0.0f;
        _states->freq_pow_incr_num = 1.0f/(float)length_sample;
        _states->wfreq_start =  2.0f * (float)M_PI * (float)_params->freq_start / (float)sample_rate;
        _states->wfreq_end =    2.0f * (float)M_PI * (float)_params->freq_stop / (float)sample_rate;
        _states->length_sample_hf = (float)length_sample;
        _states->freq_increment = _params->freq_start;
        _states->current_phase = 0.0f;
        _states->instantaneous_w =_states->wfreq_start ;
        break;
    case TSIG_CHIRP_LOGARITM:
        _states->freq_diff = (float)_params->freq_stop - (float)_params->freq_start;
        _states->freq_pow = 0.0f;
        _states->freq_pow_incr_num = 1.0f/(float)length_sample;
        _states->wfreq_start =  2.0f * (float)M_PI * _params->freq_start / (float)sample_rate;
        _states->wfreq_end =    2.0f * (float)M_PI * _params->freq_stop / (float)sample_rate;
        _states->length_sample_hf = (float)length_sample;
        _states->freq_increment = (float)_params->freq_start;
        _states->current_phase = 0.0f;
        _states->instantaneous_w =_states->wfreq_start ;
        break;
    default:
        fprintf(stderr,RED"Error: "BOLDWHITE"Wrong Chirp type.\n"RESET);
        free(_states);
        return NULL;
    }

    _states->chirp_type = _params->chirp_type_id;
    _states->audioFormat = audioFormat;
    _states->sample_increment = 0;
    _states->amplitude_coef = powf(10.0f , (0.05f * _params->amp_dB));

    return _states;
}

int32_t tsig_gen_chirp_st(uint32_t length_sample, void* states, void *audio){
    chanels_t           *_audio = (chanels_t *)audio;
    tsig_chirp_stat_t   *_st    = (tsig_chirp_stat_t *)states; 
    int                 n       = 0;

    if( (n = check_gen(length_sample, states, audio)) != 0){
        return n;
    }

    if(_st->chirp_type == TSIG_CHIRP_LINEAR){
        for (uint32_t i = 0; i < length_sample; i++){
            _audio[i].Left = sinf(_st->current_phase) * _st->amplitude_coef;
            _audio[i].Right = _audio[i].Left;
            _st->instantaneous_w = (_st->wfreq_end - _st->wfreq_start) * ((float) _st->sample_increment / _st->length_sample_hf)  +  _st->wfreq_start;
	    	_st->current_phase = fmodf (_st->current_phase + _st->instantaneous_w , 2.0f * (float)M_PI);
            _st->sample_increment++;
        }
    } else {
        for (uint32_t i = 0; i < length_sample; i++){
            _audio[i].Left = sinf(_st->current_phase) * _st->amplitude_coef;
            _audio[i].Right = _audio[i].Left;
            _st->instantaneous_w = powf(10.0, log10f (_st->wfreq_start) + (log10f (_st->wfreq_end) - log10f (_st->wfreq_start)) * ((float) _st->sample_increment / _st->length_sample_hf));
	    	_st->current_phase = fmodf (_st->current_phase + _st->instantaneous_w , 2.0f * (float)M_PI);
            _st->sample_increment++;
        }
    }

    if(_st->audioFormat == PCM){
        IEEE_754_to_PCM(audio, length_sample);
    }

    return n;
}
// pow (10.0, log10 (w0) + (log10 (w1) - log10 (w0)) * indx) ;
#include "test_sig_gen.h"

void *tsig_chirp_init_states (uint32_t sample_rate, uint32_t length_sample, void const *params){
    // sample_rate = 0;
    tsig_chirp_stat_t   *_states = NULL;
    tsig_chirp_prm_t    *_params  = (tsig_chirp_prm_t  *)params;

    _states = malloc(sizeof(tsig_chirp_stat_t));
    if(_states == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    switch (((tsig_chirp_prm_t *)(params))->chirp_type_id)
    {
    case TSIG_CHIRP_LINEAR:
        _states->freq_diff = (float)_params->freq_stop - (float)_params->freq_start;
        _states->freq_pow = 0.0f;
        _states->freq_pow_incr_num = 1.0f/(float)length_sample;//logf((float)_params->freq_stop)/logf(_states->freq_diff);
        // printf(" _states->freq_pow_incr_num %f\n", _states->freq_pow_incr_num);
        // printf(" _states->freq_pow %f\n", _states->freq_pow);
        // printf(" _states->freq_diff %f\n", _states->freq_diff);
        _states->wfreq_start =  2.0f * (float)M_PI * (float)_params->freq_start / (float)sample_rate;
        _states->wfreq_end =    2.0f * (float)M_PI * (float)_params->freq_stop / (float)sample_rate;
        _states->length_sample_hf = (float)length_sample;
        _states->freq_increment = (float)_params->freq_start;
        _states->current_phase = 0.0f;
        _states->instantaneous_w =_states->wfreq_start ;
        break;
    case TSIG_CHIRP_LOGARITM:
        _states->freq_diff = (float)_params->freq_stop - (float)_params->freq_start;
        _states->freq_pow = 0.0f;
        _states->freq_pow_incr_num = 1.0f/(float)length_sample;//logf((float)_params->freq_stop)/logf(_states->freq_diff);
        // printf(" _states->freq_pow_incr_num %f\n", _states->freq_pow_incr_num);
        // printf(" _states->freq_pow %f\n", _states->freq_pow);
        // printf(" _states->freq_diff %f\n", _states->freq_diff);
        _states->wfreq_start =  2.0f * (float)M_PI * (float)_params->freq_start / (float)sample_rate;
        _states->wfreq_end =    2.0f * (float)M_PI * (float)_params->freq_stop / (float)sample_rate;
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
  
    _states->sample_increment = 0;

    return _states;
}

int32_t tsig_gen_chirp_st(uint32_t sample_rate, uint32_t length_sample, float amplitude_coef, void const *params, void* states, void *audio){
    chanels_t  *_audio = (chanels_t *)audio;
    tsig_chirp_stat_t *_states = (tsig_chirp_stat_t *)states;
    tsig_chirp_prm_t * _params = (tsig_chirp_prm_t *)params;
    // float instantaneous_w, current_phase ;

   
	

    if(!_states){
        fprintf(stderr,RED" Error: "BOLDWHITE"NULL pointer _states.Rejected.\n"RESET);
        return -1;
    }
    if(_params->chirp_type_id == TSIG_CHIRP_LINEAR){
        for (uint32_t i = 0; i < length_sample; i++){
            _audio[i].Left = sinf(_states->current_phase) * amplitude_coef;
            _audio[i].Right = _audio[i].Left;
           
            // _states->freq_pow += _states->freq_pow_incr_num;
            // _states->freq_increment += _states->freq_increment_num;
            _states->instantaneous_w = (_states->wfreq_end - _states->wfreq_start) * ((float) _states->sample_increment / _states->length_sample_hf)  +  _states->wfreq_start;
	    	_states->current_phase = fmodf (_states->current_phase + _states->instantaneous_w , 2.0f * (float)M_PI);
            _states->sample_increment++;
        }
    } else {
        for (uint32_t i = 0; i < length_sample; i++){
            // _audio[i].Left = sinf(expf(_states->freq_increment) * (2.0f * (float)M_PI) * (float)_states->sample_increment / (float)sample_rate) * amplitude_coef;
            // _audio[i].Left = sinf((powf( _states->freq_diff,_states->freq_pow) +_states->freq_increment)  * (2.0f * (float)M_PI) * (float)_states->sample_increment / (float)sample_rate) * amplitude_coef;
            _audio[i].Left = sinf(_states->current_phase) * amplitude_coef;
            _audio[i].Right = _audio[i].Left;
           
            // _states->freq_pow += _states->freq_pow_incr_num;
            // _states->freq_increment += _states->freq_increment_num;
            _states->instantaneous_w = powf(10.0, log10f (_states->wfreq_start) + (log10f (_states->wfreq_end) - log10f (_states->wfreq_start)) * ((float) _states->sample_increment / _states->length_sample_hf));
	    	_states->current_phase = fmodf (_states->current_phase + _states->instantaneous_w , 2.0f * (float)M_PI);
            _states->sample_increment++;
        }
    }
    // return w0 + (w1 - w0) * indx ;

    return 0;
}
// pow (10.0, log10 (w0) + (log10 (w1) - log10 (w0)) * indx) ;
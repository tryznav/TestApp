#include "iir_fxd.h"

static int32_t lpf_coeff_calc(iir_prm_t *_prm, iir_coefs_t *_coeffs);
static int32_t hpf_coeff_calc(iir_prm_t *_prm, iir_coefs_t *_coeffs);
static int32_t bpf_coeff_calc(iir_prm_t *_prm, iir_coefs_t *_coeffs);
static int32_t bsf_coeff_calc(iir_prm_t *_prm, iir_coefs_t *_coeffs);
static int32_t apf_coeff_calc(iir_prm_t *_prm, iir_coefs_t *_coeffs);
static int32_t peakingEQ_coeff_calc(iir_prm_t *_prm, iir_coefs_t *_coeffs);
static int32_t lshelf_coeff_calc(iir_prm_t *_prm, iir_coefs_t *_coeffs);
static int32_t hshelf_coeff_calc(iir_prm_t *_prm, iir_coefs_t *_coeffs);

int32_t iir_coeff_calc(iir_prm_t *_prm, iir_coefs_t *_coeffs){
    switch (_prm->type)
    {
    case LPF:
        lpf_coeff_calc(_prm, _coeffs);
        return 0;
    case HPF:
        hpf_coeff_calc(_prm, _coeffs);
        return 0;
    case BPF:
        bpf_coeff_calc(_prm, _coeffs);
        return 0;
    case BSF:
        bsf_coeff_calc(_prm, _coeffs);
        return 0;
    case APF:
        apf_coeff_calc(_prm, _coeffs);
        return 0;
    case PQE:
        peakingEQ_coeff_calc(_prm, _coeffs);
        return 0;
    case LSH:
        lshelf_coeff_calc(_prm, _coeffs);
        return 0;
    case HSH:
        hshelf_coeff_calc(_prm, _coeffs);
        return 0;   
    default:
        break;
    }
    return -1;
}

static int32_t convet_coefs(iir_doub_coefs_t *coeffs_dbl, iir_coefs_t *_coeffs){
    
    coeffs_dbl->a1 = coeffs_dbl->a1 / coeffs_dbl->a0;
    coeffs_dbl->a2 = coeffs_dbl->a2 / coeffs_dbl->a0;

    coeffs_dbl->b0 = coeffs_dbl->b0 / coeffs_dbl->a0;
    coeffs_dbl->b1 = coeffs_dbl->b1 / coeffs_dbl->a0;
    coeffs_dbl->b2 =  coeffs_dbl->b2 /coeffs_dbl->a0;
    coeffs_dbl->a0 = coeffs_dbl->a0  / coeffs_dbl->a0;


    _coeffs->a0 = dbl_to_fxd_p(coeffs_dbl->a0, COEF_FRACTIONAL_BITS);
    _coeffs->a1 = dbl_to_fxd_p(coeffs_dbl->a1, COEF_FRACTIONAL_BITS);
    _coeffs->a2 = dbl_to_fxd_p(coeffs_dbl->a2, COEF_FRACTIONAL_BITS);

    _coeffs->b0 = dbl_to_fxd_p(coeffs_dbl->b0, COEF_FRACTIONAL_BITS);
    _coeffs->b1 = dbl_to_fxd_p(coeffs_dbl->b1, COEF_FRACTIONAL_BITS);
    _coeffs->b2 = dbl_to_fxd_p(coeffs_dbl->b2, COEF_FRACTIONAL_BITS);

    return 0;
}

static int32_t lpf_coeff_calc(iir_prm_t *_prm, iir_coefs_t *_coeffs){
    iir_doub_coefs_t coeffs_dbl;

    _prm->BW = BandWidth;

    _prm->f0 = _prm->cutoff_freq.whole_file;

    _prm->Q = _prm->f0 / _prm->BW;

    _prm->w = 2 * M_PI * _prm->f0 / _prm->sample_rate;

    _prm->alpha = sin(_prm->w)/(2.0 * _prm->Q);

    coeffs_dbl.a0 = 1.0 + _prm->alpha;
    coeffs_dbl.a1 = -2.0 * cos(_prm->w);
    coeffs_dbl.a2 = 1.0 - _prm->alpha;

    coeffs_dbl.b0 = (1.0 * cos(_prm->w))/2.0;
    coeffs_dbl.b1 = 1.0 * cos(_prm->w);
    coeffs_dbl.b2 = (1.0 * cos(_prm->w))/2.0;

    convet_coefs(&coeffs_dbl, _coeffs);

    return 0;
}

static int32_t hpf_coeff_calc(iir_prm_t *_prm, iir_coefs_t *_coeffs){
    iir_doub_coefs_t coeffs_dbl;

    _prm->BW = BandWidth;

    _prm->f0 = _prm->cutoff_freq.whole_file;

    _prm->Q = _prm->f0 / _prm->BW;

    _prm->w = 2 * M_PI * _prm->f0 / _prm->sample_rate;

    _prm->alpha = sin(_prm->w)/(2.0 * _prm->Q);

    coeffs_dbl.a0 = 1.0 + _prm->alpha;
    coeffs_dbl.a1 = -2.0 * cos(_prm->w);
    coeffs_dbl.a2 = 1.0 - _prm->alpha;

    coeffs_dbl.b0 = (1.0 * cos(_prm->w))/2.0;
    coeffs_dbl.b1 = -(1.0 * cos(_prm->w));
    coeffs_dbl.b2 = (1.0 * cos(_prm->w))/2.0;

    convet_coefs(&coeffs_dbl, _coeffs);

    return 0;
}

static int32_t bpf_coeff_calc(iir_prm_t *_prm, iir_coefs_t *_coeffs){
    iir_doub_coefs_t coeffs_dbl;

    _prm->BW = (double)(_prm->cutoff_freq.sweep.end - _prm->cutoff_freq.sweep.start);
    _prm->f0 =  (double)(_prm->cutoff_freq.sweep.end + _prm->cutoff_freq.sweep.start) / 2.0;

    _prm->Q = _prm->f0 / _prm->BW;
_prm->Q = 1.0;
    _prm->w = 2 * M_PI * _prm->f0 / _prm->sample_rate;
    _prm->alpha = sin(_prm->w)/(2.0 * _prm->Q);

    coeffs_dbl.a0 = 1.0 + _prm->alpha;
    coeffs_dbl.a1 = -2.0 * cos(_prm->w);
    coeffs_dbl.a2 = 1.0 - _prm->alpha;

    coeffs_dbl.b0 = _prm->alpha;
    coeffs_dbl.b1 = 0.0f;
    coeffs_dbl.b2 = -_prm->alpha;
      printf("bpf_coeff_calc");
    convet_coefs(&coeffs_dbl, _coeffs);

    return 0;
}

static int32_t apf_coeff_calc(iir_prm_t *_prm, iir_coefs_t *_coeffs){
    iir_doub_coefs_t coeffs_dbl;

    _prm->BW = (double)(_prm->cutoff_freq.sweep.end - _prm->cutoff_freq.sweep.start);
    _prm->BW = 1.0; //BandWidth;
    _prm->f0 = _prm->cutoff_freq.whole_file;
    _prm->Q = _prm->f0 / _prm->BW;

    _prm->w = 2 * M_PI * _prm->f0 / _prm->sample_rate;
    _prm->alpha = sin(_prm->w)/(2.0 * _prm->Q);

    coeffs_dbl.a0 = 1.0 + _prm->alpha;
    coeffs_dbl.a1 = -2.0 * cos(_prm->w);
    coeffs_dbl.a2 = 1.0 - _prm->alpha;

    coeffs_dbl.b0 = 1.0 - _prm->alpha;
    coeffs_dbl.b1 = -2.0 * cos(_prm->w);
    coeffs_dbl.b2 = 1.0 - _prm->alpha;
     printf("apf_coeff_calc");
    convet_coefs(&coeffs_dbl, _coeffs);

    return 0;
}

static int32_t bsf_coeff_calc(iir_prm_t *_prm, iir_coefs_t *_coeffs){
    iir_doub_coefs_t coeffs_dbl;

    _prm->BW = (double)(_prm->cutoff_freq.sweep.end - _prm->cutoff_freq.sweep.start);
    _prm->f0 =  (double)(_prm->cutoff_freq.sweep.end + _prm->cutoff_freq.sweep.start) / 2.0;

    _prm->Q = _prm->f0 / _prm->BW;

    _prm->w = 2 * M_PI * _prm->f0 / _prm->sample_rate;
    _prm->alpha = sin(_prm->w)/(2.0 * _prm->Q);

    coeffs_dbl.a0 = 1.0 + _prm->alpha;
    coeffs_dbl.a1 = -2.0 * cos(_prm->w);
    coeffs_dbl.a2 = 1.0 - _prm->alpha;

    coeffs_dbl.b0 = 1.0;
    coeffs_dbl.b1 = -2.0 * cos(_prm->w);
    coeffs_dbl.b2 = 1.0;
     printf("bsf_coeff_calc");
    convet_coefs(&coeffs_dbl, _coeffs);

    return 0;
}

static int32_t peakingEQ_coeff_calc(iir_prm_t *_prm, iir_coefs_t *_coeffs){
    iir_doub_coefs_t coeffs_dbl;
    _prm->A = pow(10.0, _prm->gain_dB / 40.0);
    _prm->f0 = (double)(_prm->cutoff_freq.whole_file);
    
    _prm->BW = BandWidth;
    _prm->Q = _prm->f0 / _prm->BW;

    _prm->w = 2 * M_PI * _prm->f0 / _prm->sample_rate;
    _prm->alpha = sin(_prm->w)/(2.0 * _prm->Q);

    coeffs_dbl.a0 = 1.0 + (_prm->alpha / _prm->A);
    coeffs_dbl.a1 = -2.0 * cos(_prm->w);
    coeffs_dbl.a2 = 1.0 - (_prm->alpha / _prm->A);

    coeffs_dbl.b0 = 1.0 + (_prm->alpha * _prm->A);
    coeffs_dbl.b1 = -2.0 * cos(_prm->w);
    coeffs_dbl.b2 = 1.0 - (_prm->alpha * _prm->A);

    convet_coefs(&coeffs_dbl, _coeffs);
    printf("peakingEQ_coeff_calc\n");

    return 0;
}

static int32_t lshelf_coeff_calc(iir_prm_t *_prm, iir_coefs_t *_coeffs){
    iir_doub_coefs_t coeffs_dbl;
    _prm->A = pow(10.0, _prm->gain_dB / 40.0);
    _prm->f0 = (double)(_prm->cutoff_freq.sweep.end + _prm->cutoff_freq.sweep.start) / 2.0;
    
    _prm->BW = BandWidth;
    _prm->Q = _prm->f0 / _prm->BW;

    _prm->w = 2 * M_PI * _prm->f0 / _prm->sample_rate;
    _prm->alpha = sin(_prm->w)/(2.0 * _prm->Q);

    coeffs_dbl.a0 = (_prm->A + 1.0) + ((_prm->A - 1.0) * cos(_prm->w)) + (2.0 * pow(_prm->A, 0.5));
    coeffs_dbl.a1 = -2.0 * ((_prm->A - 1.0) + (_prm->A + 1.0) * cos(_prm->w));
    coeffs_dbl.a2 = (_prm->A + 1.0) + ((_prm->A - 1.0) * cos(_prm->w)) - (2.0 * pow(_prm->A, 0.5));

    coeffs_dbl.b0 = _prm->A * ((_prm->A + 1.0) + ((_prm->A - 1.0) * cos(_prm->w)) + (2.0 * pow(_prm->A, 0.5)));
    coeffs_dbl.b1 = 2.0 * _prm->A * ((_prm->A - 1.0) - (_prm->A + 1.0) * cos(_prm->w));
    coeffs_dbl.b2 = _prm->A * ((_prm->A + 1.0) + ((_prm->A - 1.0) * cos(_prm->w)) - (2.0 * pow(_prm->A, 0.5)));

    convet_coefs(&coeffs_dbl, _coeffs);

    return 0;
}

static int32_t hshelf_coeff_calc(iir_prm_t *_prm, iir_coefs_t *_coeffs){
    iir_doub_coefs_t coeffs_dbl;
    _prm->A = pow(10.0, _prm->gain_dB / 40.0);
    _prm->f0 = (double)(_prm->cutoff_freq.sweep.end + _prm->cutoff_freq.sweep.start) / 2.0;
    
    _prm->BW = BandWidth;
    _prm->Q = _prm->f0 / _prm->BW;

    _prm->w = 2 * M_PI * _prm->f0 / _prm->sample_rate;
    _prm->alpha = sin(_prm->w)/(2.0 * _prm->Q);

    coeffs_dbl.a0 = (_prm->A + 1.0) + ((_prm->A - 1.0) * cos(_prm->w)) + (2.0 * pow(_prm->A, 0.5));
    coeffs_dbl.a1 = -2.0 * ((_prm->A - 1.0) + (_prm->A + 1.0) * cos(_prm->w));
    coeffs_dbl.a2 = (_prm->A + 1.0) + ((_prm->A - 1.0) * cos(_prm->w)) - (2.0 * pow(_prm->A, 0.5));

    coeffs_dbl.b0 = _prm->A * ((_prm->A + 1.0) + ((_prm->A - 1.0) * cos(_prm->w)) + (2.0 * pow(_prm->A, 0.5)));
    coeffs_dbl.b1 = -2.0 * _prm->A * ((_prm->A - 1.0) + (_prm->A + 1.0) * cos(_prm->w));
    coeffs_dbl.b2 = _prm->A * ((_prm->A + 1.0) + ((_prm->A - 1.0) * cos(_prm->w)) - (2.0 * pow(_prm->A, 0.5)));

    convet_coefs(&coeffs_dbl, _coeffs);

    return 0;
}

#include "cross_flt.h"

static audio_type apf_flt_1st(audio_type x, apf_states_1st_t *st, apf_coef_1st_t *coef);
static audio_type apf_flt_2nd(audio_type x, apf_states_2nd_t *st, apf_coef_2nd_t *coef);

bands_t crossover_flt(audio_type x, cross_flt_states_t *st, cross_flt_coef_t *coef){
    bands_t res;

    audio_type  apl_2nd = apf_flt_2nd(x,       &(st->apf_2nd[0]), &coef->apf_2nd[1]);
    audio_type  apl_1st = apf_flt_1st(x,       &(st->apf_1st[0]), &coef->apf_1st[1]);
    audio_type  l_f2 = flt_add(apl_2nd, apl_1st) * 0.5f;
        // apl_1st = apf_flt_1st(apl_1st,       &(st->apf_1st[3]), &coef->apf_1st[1]);
        apl_2nd = apf_flt_2nd(l_f2, &(st->apf_2nd[3]), &coef->apf_2nd[1]);
        apl_1st = apf_flt_1st(l_f2, &(st->apf_1st[3]), &coef->apf_1st[1]);     
        
        l_f2 = flt_add(apl_2nd, apl_1st) * 0.5f;
    audio_type h_f2 = x - l_f2;
    // res.band1 = l_f2;//flt_add(x, apl_1st) * 0.5f;
    
    apl_2nd = apf_flt_2nd(l_f2, &(st->apf_2nd[1]), &coef->apf_2nd[0]);   
    apl_1st = apf_flt_1st(l_f2, &(st->apf_1st[1]), &coef->apf_1st[0]);

    res.band1 = flt_add(apl_2nd, apl_1st) * 0.5f;

        apl_2nd = apf_flt_2nd(res.band1, &(st->apf_2nd[4]), &coef->apf_2nd[0]);
        apl_1st = apf_flt_1st(res.band1, &(st->apf_1st[4]), &coef->apf_1st[0]);

    res.band1 = flt_add(apl_2nd, apl_1st) * 0.5f;

    res.band2 = l_f2 - res.band1;

    apl_2nd = apf_flt_2nd(h_f2, &(st->apf_2nd[2]), &coef->apf_2nd[2]);   
    apl_1st = apf_flt_1st(h_f2, &(st->apf_1st[2]), &coef->apf_1st[2]);

    res.band3 = flt_add(apl_2nd, apl_1st) * 0.5f;

        apl_2nd = apf_flt_2nd(res.band3, &(st->apf_2nd[5]), &coef->apf_2nd[2]);
        apl_1st = apf_flt_1st(res.band3, &(st->apf_1st[5]), &coef->apf_1st[2]);

    res.band3 = flt_add(apl_2nd, apl_1st) * 0.5f;
    res.band4 = h_f2 - res.band3;

    return res;
}

//Direct form 2
static audio_type apf_flt_1st(audio_type x, apf_states_1st_t *st, apf_coef_1st_t *coef){
    audio_type xh = flt_msub(x, coef->k1, st->xh[0]);
    audio_type y =  flt_mac(st->xh[0], coef->k1, xh);

    st->xh[0] = xh;
    return y;
}

//Direct form 2 Transposed
static audio_type apf_flt_2nd(audio_type x, apf_states_2nd_t *st, apf_coef_2nd_t *coef){
    audio_type  y = flt_mac(st->xh[0], coef->k2, x);
    
    st->xh[0] = flt_mac(st->xh[1], coef->k1, x);
    st->xh[0] = flt_msub(st->xh[0], coef->k1, y);

    st->xh[1] = flt_msub(x, coef->k2, y);

    return y;
}

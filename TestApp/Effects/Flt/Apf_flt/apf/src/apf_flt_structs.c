#include "apf_flt.h"

// First order All-Pass Filter

audio_type apf_flt_1st_DirectI(audio_type x, apf_states_t *st, apf_coef_t *coef){
    audio_type y = 0;
    y = flt_mac(y, coef->k[0], x);
    y = flt_msub(y, coef->k[0], st->y[0]);
    y = flt_add(y, st->xh[0]);

    st->xh[0] = x;
    st->y[0] = y;
    return y;
}

audio_type apf_flt_1st_DirectII(audio_type x, apf_states_t *st, apf_coef_t *coef){
    audio_type xh = flt_msub(x, coef->k[0], st->xh[0]);
    audio_type y =  flt_mac(st->xh[0], coef->k[0], xh);

    st->xh[0] = xh;
    // printf("xh = %f\n", xh);
    return y;
}

//+++++++++++Second order All-Pass Filter+++++++++++

//Direct Forms

audio_type apf_flt_2nd_DirectI(audio_type x, apf_states_t *st, apf_coef_t *coef){
    audio_type  y = 0.0f;

    y = flt_mac(y, coef->k[0], st->xh[0]);
    y = flt_msub(y, coef->k[0], st->y[0]);

    y = flt_mac(y, coef->k[1], x);
    y = flt_msub(y, coef->k[1], st->y[1]);
    y = flt_add(y, st->xh[1]);

    st->xh[1] = st->xh[0];
    st->xh[0] = x;

    st->y[1] = st->y[0];
    st->y[0] = y;
    return y;
}

audio_type apf_flt_2nd_DirectI_t(audio_type x, apf_states_t *st, apf_coef_t *coef){
    audio_type xh = flt_add(x, st->xh[0]);
    st->xh[0] = flt_msub(st->xh[1], coef->k[0], xh);
    st->xh[1] = flt_neg(flt_mul(xh, coef->k[1]));

    audio_type y = flt_mac(st->y[0], coef->k[1], xh);

    st->y[0] = flt_mac(st->y[1], coef->k[0], xh);
    st->y[1] = xh;
    
    return y;
}

audio_type apf_flt_2nd_DirectII(audio_type x, apf_states_t *st, apf_coef_t *coef){
    audio_type  xh = 0;
    xh = flt_msub(xh, coef->k[1], st->xh[1]);
    xh = flt_msub(xh, coef->k[0], st->xh[0]);
    xh  += x;

    audio_type  y = flt_mul(coef->k[1], xh);

    y = flt_mac(y, coef->k[0], st->xh[0]);
    y = flt_add(y, st->xh[1]);

    st->xh[1] = st->xh[0];
    st->xh[0] = xh;

    return y;
}

audio_type apf_flt_2nd_DirectII_t(audio_type x, apf_states_t *st, apf_coef_t *coef){
    audio_type  y = flt_mac(st->xh[0], coef->k[1], x);
    
    st->xh[0] = flt_mac(st->xh[1], coef->k[0], x);
    st->xh[0] = flt_msub(st->xh[0], coef->k[0], y);

    st->xh[1] = flt_msub(x, coef->k[1], y);

    return y;
}

//Nested Forms

audio_type apf_2nd_Lattice(audio_type x, apf_states_t *st, apf_coef_t *coef){
    audio_type  xh = flt_msub(x, coef->k[1], st->xh[1]);
    audio_type  y = flt_mac(st->xh[1], coef->k[1], xh);

    st->xh[1] = apf_flt_1st_DirectII(xh, st, coef);

    return y;
}

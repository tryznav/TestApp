#include "apf_flt.h"

// First order All-Pass Filter

audio_type apl_1st_DirectI(audio_type x, apf_states_t *st, apf_coefs_t *coef){
    audio_type xh = flt_msub(x, coef->k[0], st->xh[0]);
    audio_type y =  flt_mac(st->xh[0], coef->k[0], xh);

    st->xh[0] = xh;

    return y;
}

audio_type apl_1st_DirectII(audio_type x, apf_states_t *st, apf_coefs_t *coef){
    audio_type xh = flt_msub(x, coef->k[0], st->xh[0]);
    audio_type y =  flt_mac(st->xh[0], coef->k[0], xh);

    st->xh[0] = xh;

    return y;
}

//+++++++++++Second order All-Pass Filter+++++++++++

audio_type apl_direct_f1_t_2nd(audio_type x, apf_states_t *st, apf_coefs_t *coef){
    audio_type xh = flt_add(x, st->xh[0]);
    st->xh[0] = flt_msub(st->xh[1], coef->c[0], xh);
    st->xh[1] = flt_neg(flt_mul(xh, coef->c[1]));

    audio_type y = flt_mac(st->y[0], coef->c[1], xh);

    st->y[0] = flt_mac(st->y[1], coef->c[0], xh);
    st->y[1] = xh;
    
    return y;
}



audio_type apl_direct_f2_2nd(audio_type x, apf_states_t *st, apf_coefs_t *coef){
    audio_type  xh = 0;
    xh = flt_msub(xh, coef->c[1], st->xh[1]);
    xh = flt_msub(xh, coef->c[0], st->xh[0]);
    xh  += x;

    //  printf("xh %f\n", xh);  
    audio_type  y = flt_mul(coef->c[1], xh);
//  

    y = flt_mac(y, coef->c[0], st->xh[0]);
    y = flt_add(y, st->xh[1]);

    st->xh[1] = st->xh[0];
    st->xh[0] = xh;

    return y;
}
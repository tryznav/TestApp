#include "cross_flt.h"

audio_type calc_crossover_dbl(audio_type x, cross_states_t *st, coef_t *coef){
    double band1 = 0.0f;
    double band2 = 0.0f;
    double band3 = 0.0f;
    double band4 = 0.0f;

    //calculate for f2
    double  apl_2nd = apl_2nd_dbl(x,       &(st->apf_2nd[0]), &coef->apf_2nd[1]);
    double  apl_1st = apl_1st_dbl(x,       &(st->apf_1st[0]), &coef->apf_1st[1]);
    double  l_f2 = (apl_2nd + apl_1st)*0.5;

        apl_2nd = apl_2nd_dbl(l_f2, &(st->apf_2nd[3]), &coef->apf_2nd[1]);
        apl_1st = apl_1st_dbl(l_f2, &(st->apf_1st[3]), &coef->apf_1st[1]);     
        
        l_f2 = (apl_2nd + apl_1st)* 0.5f;
    double h_f2 = x - l_f2;


    apl_2nd = apl_2nd_dbl(l_f2, &(st->apf_2nd[1]), &coef->apf_2nd[0]);   
    apl_1st = apl_1st_dbl(l_f2, &(st->apf_1st[1]), &coef->apf_1st[0]);

    band1 = flt_add(apl_2nd, apl_1st) * 0.5f;

        apl_2nd = apl_2nd_dbl(band1, &(st->apf_2nd[4]), &coef->apf_2nd[1]);
        apl_1st = apl_1st_dbl(band1, &(st->apf_1st[4]), &coef->apf_1st[1]);

    band1 = flt_add(apl_2nd, apl_1st) * 0.5f;

    band2 = l_f2 - band1;//flt_sub(apl_2nd, apl_1st) * 0.5f;

    apl_2nd = apl_2nd_dbl(h_f2, &(st->apf_2nd[2]), &coef->apf_2nd[2]);   
    apl_1st = apl_1st_dbl(h_f2, &(st->apf_1st[2]), &coef->apf_1st[2]);

    band3 = flt_add(apl_2nd, apl_1st) * 0.5f;

        apl_2nd = apl_2nd_order_dbl(band3, &(st->apf_2nd[5]), &coef->apf_2nd[1]);
        apl_1st = apl_1st_order_dbl(band3, &(st->apf_1st[5]), &coef->apf_1st[1]);

    band3 = flt_add(apl_2nd, apl_1st) * 0.5f;
    band4 = h_f2 - band3;// flt_sub(apl_2nd, apl_1st) * 0.5f;


    audio_type res = 0;
    res = flt_mac(res, band1, coef->gain_c[0]);
    res = flt_mac(res, band2, coef->gain_c[1]);
    res = flt_mac(res, band3, coef->gain_c[2]);
    res = flt_mac(res, band4, coef->gain_c[3]);

    return res;
}

static double apl_1st_dbl(double x, apf_states_t *st, apf_coef_t *coef){
    double xh = x;
    xh -= coef->k_dbl[0] * st->xh_dbl[0];

    double y = st->xh_dbl[0] + (xh * coef->k_dbl[0]);

    st->xh_dbl[0] = xh;

    return y;
}

static audio_type apl_direct_dbl(audio_type x, apf_states_t *st, apf_coef_t *coef){
    double xh = x;

    xh -= (coef->k_dbl[0] * st->xh_dbl[0]);
    xh -= (coef->k_dbl[1] * st->xh_dbl[1]);
    // printf("xh dbl %f\n", xh);

    double y = st->xh_dbl[1];
    y += coef->k_dbl[1] * xh;
    y += coef->k_dbl[0] * st->xh_dbl[0];

    st->xh_dbl[1] = st->xh_dbl[0];
    st->xh_dbl[0] = xh;

    return (audio_type)y;
}
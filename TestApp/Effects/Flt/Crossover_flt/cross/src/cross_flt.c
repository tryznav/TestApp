#include "cross_flt.h"

bands_t crossover_flt(audio_type x, cross_states_t *st, cross_coef_t *coef){
    bands_t res;

    //calculate for f2
    audio_type  apl_2nd = coef->apf_2nd_f(x,       &(st->apf_2nd[0]), &coef->apf_2nd[1]);
    audio_type  apl_1st = coef->apf_1st_f(x,       &(st->apf_1st[0]), &coef->apf_1st[1]);
    audio_type  l_f2 = flt_add(apl_2nd, apl_1st) * 0.5f;

        apl_2nd = coef->apf_2nd_f(l_f2, &(st->apf_2nd[3]), &coef->apf_2nd[1]);
        apl_1st = coef->apf_1st_f(l_f2, &(st->apf_1st[3]), &coef->apf_1st[1]);     
        
        l_f2 = flt_add(apl_2nd, apl_1st) * 0.5f;
    audio_type l = l_f2 * 0.5f;
    audio_type h_f2 = x - l_f2;
    audio_type h = h_f2 * 0.5f;
        // res.band1 = apl_2nd;
        // res.band2 = apl_1st;
    
    apl_2nd = coef->apf_2nd_f(l_f2, &(st->apf_2nd[1]), &coef->apf_2nd[0]);   
    apl_1st = coef->apf_1st_f(l_f2, &(st->apf_1st[1]), &coef->apf_1st[0]);

    res.band1 = flt_add(apl_2nd, apl_1st) * 0.5f;

        apl_2nd = coef->apf_2nd_f(res.band1, &(st->apf_2nd[4]), &coef->apf_2nd[1]);
        apl_1st = coef->apf_1st_f(res.band1, &(st->apf_1st[4]), &coef->apf_1st[1]);

    res.band1 = flt_add(apl_2nd, apl_1st) * 0.5f;

    res.band2 = l_f2 - res.band1;

    apl_2nd = coef->apf_2nd_f(h_f2, &(st->apf_2nd[2]), &coef->apf_2nd[2]);   
    apl_1st = coef->apf_1st_f(h_f2, &(st->apf_1st[2]), &coef->apf_1st[2]);

    res.band3 = flt_add(apl_2nd, apl_1st) * 0.5f;

        apl_2nd = coef->apf_2nd_f(res.band3, &(st->apf_2nd[5]), &coef->apf_2nd[1]);
        apl_1st = coef->apf_1st_f(res.band3, &(st->apf_1st[5]), &coef->apf_1st[1]);

    res.band3 = flt_add(apl_2nd, apl_1st) * 0.5f;
    res.band4 = h_f2 - res.band3;

    return res;
}

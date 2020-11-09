#include "cross_flt.h"

bands_dbl_t crossover_dbl(double x, cross_states_t *st, cross_coef_t *coef){
    bands_dbl_t res;
    //calculate for f2
    double  apl_2nd = coef->apf_2nd_f_dbl(x,       &(st->apf_2nd[0]), &coef->apf_2nd[1]);
    double  apl_1st = coef->apf_1st_f_dbl(x,       &(st->apf_1st[0]), &coef->apf_1st[1]);
    double  l_f2 = (apl_2nd + apl_1st)*0.5;

        apl_2nd = coef->apf_2nd_f_dbl(l_f2, &(st->apf_2nd[3]), &coef->apf_2nd[1]);
        apl_1st = coef->apf_1st_f_dbl(l_f2, &(st->apf_1st[3]), &coef->apf_1st[1]);     
        
        l_f2 = (apl_2nd + apl_1st) * 0.5;
    double h_f2 = x - l_f2;

    apl_2nd = coef->apf_2nd_f_dbl(l_f2, &(st->apf_2nd[1]), &coef->apf_2nd[0]);   
    apl_1st = coef->apf_1st_f_dbl(l_f2, &(st->apf_1st[1]), &coef->apf_1st[0]);

    res.band1 = (apl_2nd + apl_1st) * 0.5;

        apl_2nd = coef->apf_2nd_f_dbl(res.band1, &(st->apf_2nd[4]), &coef->apf_2nd[1]);
        apl_1st = coef->apf_1st_f_dbl(res.band1, &(st->apf_1st[4]), &coef->apf_1st[1]);

    res.band1 = (apl_2nd + apl_1st) * 0.5;

    res.band2 = l_f2 - res.band1;//flt_sub(apl_2nd, apl_1st) * 0.5f;

    apl_2nd = coef->apf_2nd_f_dbl(h_f2, &(st->apf_2nd[2]), &coef->apf_2nd[2]);   
    apl_1st = coef->apf_1st_f_dbl(h_f2, &(st->apf_1st[2]), &coef->apf_1st[2]);

    res.band3 = ( apl_2nd +  apl_1st) * 0.5;

        apl_2nd = coef->apf_2nd_f_dbl(res.band3, &(st->apf_2nd[5]), &coef->apf_2nd[1]);
        apl_1st = coef->apf_1st_f_dbl(res.band3, &(st->apf_1st[5]), &coef->apf_1st[1]);

    res.band3 = (apl_2nd + apl_1st) * 0.5;
    res.band4 = h_f2 - res.band3;// flt_sub(apl_2nd, apl_1st) * 0.5f;




    return res;
}



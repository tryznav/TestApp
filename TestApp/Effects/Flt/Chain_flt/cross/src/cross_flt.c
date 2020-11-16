#include "cross_flt.h"


static band2_t cross2b_flt(audio_type x, cross2b_states_t *st, cross_flt_coef_t *coef);
static audio_type compes_flt(audio_type x, co_states_t *st, cross_flt_coef_t *coef);

band4_t cross4b_flt(audio_type x, cross4b_states_t *st, cross_flt_coef_t *coef){
    band4_t res;
    band2_t f1 = cross2b_flt(x, &st->f1, &coef[1]);
    f1.band1 = compes_flt(f1.band1, &st->c_f2, &coef[2]);
    f1.band2 = compes_flt(f1.band2, &st->c_f0, &coef[0]);
    
    res.low = cross2b_flt(f1.band1, &st->f0, &coef[0]);
    res.high = cross2b_flt(f1.band2, &st->f2, &coef[2]);
    // res.low = cross2b_flt(x, &st->f1, &coef[1]);
    // res.low = f1;
    // res.high.band1 = 0.0f;
    // res.high.band2 = 0.0f;
    return res;
}


static band2_t cross2b_flt(audio_type x, cross2b_states_t *st, cross_flt_coef_t *coef){
    band2_t res;
    my_float y_2nd = flt_mac(st->s_1[0], coef->k2_2, x);

    st->s_1[0] = flt_mac(st->s_1[1], coef->k1_2, x);
    st->s_1[0] = flt_msub(st->s_1[0], coef->k1_2, y_2nd);

    st->s_1[1] = flt_msub(x, coef->k2_2, y_2nd);
/*******************************************************/
    //first order
    my_float f = flt_msub(x, coef->k1_1, st->f_1);
    my_float y_1st =  flt_mac(st->f_1, coef->k1_1, f);

    st->f_1 = f;
    // printf("coef[i].k1_1 %f\n", coef->k1_1);
/*******************************************************/
    //get low 
    res.band1 = flt_mul(flt_add(y_2nd, y_1st), 0.5f);
    // res.band2 = x - res.band1;
/*******************************************************/
    //1st order compesation
    f = flt_msub(y_2nd, coef->k1_1, st->f_c);
    y_1st =  flt_mac(st->f_c, coef->k1_1, f);

    st->f_c = f;
    res.band2 = y_1st;
    // res.band2  = x;
/*******************************************************/
    //second order
    y_2nd = flt_mac(st->s_2[0], coef->k2_2, res.band1);
    
    st->s_2[0] = flt_mac(st->s_2[1], coef->k1_2, res.band1);
    st->s_2[0] = flt_msub(st->s_2[0], coef->k1_2, y_2nd);

    st->s_2[1] = flt_msub(res.band1, coef->k2_2, y_2nd);
/*******************************************************/
    //first order
    f = flt_msub(res.band1, coef->k1_1, st->f_2);
    y_1st =  flt_mac(st->f_2, coef->k1_1, f);

    st->f_2 = f;


    res.band1 = flt_mul(flt_add(y_2nd, y_1st), 0.5f);
    res.band2 = res.band2 - res.band1;

    return res;
}

//compensation
static audio_type compes_flt(audio_type x, co_states_t *st, cross_flt_coef_t *coef){
    audio_type y = flt_mac(st->s[0], coef->k2_2, x);
    
    st->s[0] = flt_mac(st->s[1], coef->k1_2, x);
    st->s[0] = flt_msub(st->s[0], coef->k1_2, y);

    st->s[1] = flt_msub(x, coef->k2_2 ,y);
/*******************************************************/
    //first order
    audio_type f = flt_msub(y, coef->k1_1, st->f);
    y = flt_mac(st->f, coef->k1_1, f);
    st->f = f;

    return y;
}

// bands_t crossover_flt(audio_type x, cross_flt_states_t *st, cross_flt_coef_t *coef){
//     bands_t res;
//     audio_type xh = 0.0; 
//     audio_type  apl_2nd = apf_flt_2nd(x,       &(st->apf_2nd[0]), &coef->apf_2nd[1]);
//     audio_type  apl_1st = apf_flt_1st(x,       &(st->apf_1st[0]), &coef->apf_1st[1]);
//     audio_type  l_f2 = flt_add(apl_2nd, apl_1st) * 0.5f;
//     audio_type h_f2 = 0.0f;

// xh = apl_2nd;
// xh = apf_flt_1st(xh,       &(st->apf_1st[1]), &coef->apf_1st[1]);

//         apl_2nd = apf_flt_2nd(l_f2, &(st->apf_2nd[1]), &coef->apf_2nd[1]);
//         apl_1st = apf_flt_1st(l_f2, &(st->apf_1st[2]), &coef->apf_1st[1]);

//     l_f2 = flt_add(apl_2nd, apl_1st) * 0.5f;
//     h_f2 = xh - l_f2;

// //compensation f2

// l_f2 = apf_flt_2nd(l_f2, &(st->apf_2nd[7]), &coef->apf_2nd[2]);
// l_f2 = apf_flt_1st(l_f2, &(st->apf_1st[10]), &coef->apf_1st[2]);

// /****************************************************************/
    
//     apl_2nd = apf_flt_2nd(l_f2, &(st->apf_2nd[2]), &coef->apf_2nd[0]);   
//     apl_1st = apf_flt_1st(l_f2, &(st->apf_1st[3]), &coef->apf_1st[0]);
// xh = apl_2nd;
// xh = apf_flt_1st(xh,       &(st->apf_1st[4]), &coef->apf_1st[0]);
//     res.band1 = flt_add(apl_2nd, apl_1st) * 0.5f;

//         apl_2nd = apf_flt_2nd(res.band1, &(st->apf_2nd[3]), &coef->apf_2nd[0]);
//         apl_1st = apf_flt_1st(res.band1, &(st->apf_1st[5]), &coef->apf_1st[0]);

//     res.band1 = flt_add(apl_2nd, apl_1st) * 0.5f;
//     res.band2 = xh - res.band1;

// //compensation f0

// h_f2 = apf_flt_2nd(h_f2, &(st->apf_2nd[4]), &coef->apf_2nd[0]);
// h_f2 = apf_flt_1st(h_f2, &(st->apf_1st[6]), &coef->apf_1st[0]);

// /****************************************************************/

//     apl_2nd = apf_flt_2nd(h_f2, &(st->apf_2nd[5]), &coef->apf_2nd[2]);   
//     apl_1st = apf_flt_1st(h_f2, &(st->apf_1st[7]), &coef->apf_1st[2]);
// xh = apl_2nd;
// xh = apf_flt_1st(xh,       &(st->apf_1st[8]), &coef->apf_1st[2]);
//     res.band3 = flt_add(apl_2nd, apl_1st) * 0.5f;

//         apl_2nd = apf_flt_2nd(res.band3, &(st->apf_2nd[6]), &coef->apf_2nd[2]);
//         apl_1st = apf_flt_1st(res.band3, &(st->apf_1st[9]), &coef->apf_1st[2]);

//     res.band3 = flt_add(apl_2nd, apl_1st) * 0.5f;
//     res.band4 = xh - res.band3;

//     return res;
// }

// //Direct form 2
// static audio_type apf_flt_1st(audio_type x, apf_states_1st_t *st, apf_coef_1st_t *coef){
//     audio_type xh = flt_msub(x, coef->k1, st->xh[0]);
//     audio_type y =  flt_mac(st->xh[0], coef->k1, xh);

//     st->xh[0] = xh;
//     return y;
// }

// //Direct form 2 Transposed
// static audio_type apf_flt_2nd(audio_type x, apf_states_2nd_t *st, apf_coef_2nd_t *coef){
//     audio_type  y = flt_mac(st->xh[0], coef->k2, x);
    
//     st->xh[0] = flt_mac(st->xh[1], coef->k1, x);
//     st->xh[0] = flt_msub(st->xh[0], coef->k1, y);

//     st->xh[1] = flt_msub(x, coef->k2, y);

//     return y;
// }
#include "cross.h"

// #if CH == 2
//     const vfloat HALF_A = {0.5f, 0.5f};
// #endif

static inline void cross2b_process(vfloat *x, band2_t *res, cross2b_coef_t *coef, cross2b_states_t *st);
static inline void compes_process(vfloat *x, cross2b_coef_t *coef, co_states_t *st);

void cross4b_process(vfloat *x, band4_t *res, cross4b_coef_t *coef, cross4b_states_t *st){
    band2_t tmp;
    cross2b_process(x, &tmp,  &coef->f[1], &st->f1);
    //     printf(" tmp.band1 %f\n", tmp.band1.ch[1]);
    // printf(" tmp.band2 %f\n", tmp.band2.ch[1]);
    // exit(0);
    compes_process(&tmp.band1,  &coef->f[2], &st->c_f2);
    // compes_process(&tmp.band2, &coef->f[0],  &st->c_f0);

    cross2b_process(&tmp.band1, &res->low, &coef->f[0], &st->f0);
    cross2b_process(&tmp.band2, &res->high, &coef->f[2], &st->f2);
}


static inline void cross2b_process(vfloat *x, band2_t *res, cross2b_coef_t *coef, cross2b_states_t *st){
    // printf(" tmp.band1 %f\n", x->ch[1]);
    // printf(" tmp.band2 %f\n", x->ch[1]);

    // printf(" tmp.band1 2 %f\n", x->ch[1]);
    // printf(" tmp.band22 %f\n", x->ch[1]);
    *x = vmul(*x , HALF_A);
    vfloat y_2nd = vmac(coef->k2, *x, st->xh1);

    st->xh1 = vmsub(coef->k1, y_2nd, st->xh2);
    st->xh1 = vmac(coef->k1, *x, st->xh1);

    st->xh2 = vmsub(coef->k2, y_2nd, *x);
/*******************************************************/
    //first order
    vfloat f = vmsub(coef->k, st->xh, *x);
    vfloat y_1st =  vmac(coef->k, f , st->xh);

    st->xh = f;

/*******************************************************/
    //get low 
    res->band1 = vadd(y_2nd, y_1st);
    // res->band1 = vmul(res->band1 , HALF_A);
    // y_2nd = vmul(y_2nd , TWO_A);
    res->band2 = vsub(y_2nd, y_1st);
    // printf(" tmp.band1 %f\n", res->band1.ch[1]);
    // printf(" tmp.band2 %f\n", res->band2.ch[1]);
    


}

//compensation
static inline void compes_process(vfloat *x, cross2b_coef_t *coef, co_states_t *st){
    
    vfloat y_2nd = vmac(coef->k2, *x, st->xh1);

    st->xh1 = vmsub(coef->k1, y_2nd, st->xh2);
    st->xh1 = vmac(coef->k1, *x, st->xh1);

    st->xh2 = vmsub(coef->k2, y_2nd, *x);
    
    *x = y_2nd;
}

// bands_t crossover_flt(vfloat x, cross_flt_states_t *st, cross_flt_coef_t *coef){
//     bands_t res;
//     vfloat xh = 0.0; 
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
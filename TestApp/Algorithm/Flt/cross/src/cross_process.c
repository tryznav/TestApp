#include "cross.h"

#if CH == 2
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
    *x = vmul(*x ,coef->half);
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
#endif
#if CH == 8
static inline void cross2b_process(vfloat *x, band2_t *res, cross2b_coef_t *coef, cross2b_states_t *st);
static inline void cross2b_to_4b_process(vfloat *x, band4_t *res, cross2b_to_4b_coef_t *coef, cross2b_to_4b_states_t *st);
//ch[0,1] low_low; ch[2,3] high_low ch[4,5] low_high ch[4,5] high_high
void cross4b_process(vfloat *x, band4_t *res, cross4b_coef_t *coef, cross4b_states_t *st){
    band2_t tmp;
    cross2b_process(x, &tmp,  &coef->f1, &st->f1);
    // *res = tmp;
    cross2b_to_4b_process(&tmp, res, &coef->f0_f2, &st->f0_f2);
}


static inline void cross2b_process(vfloat *x, band2_t *res, cross2b_coef_t *coef, cross2b_states_t *st){
    *x = vmul(*x ,coef->half);
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
    f = vneg(y_1st);
    y_1st.ch[2] = f.ch[0];
    y_1st.ch[3] = f.ch[1];
    y_2nd.ch[2] = y_2nd.ch[0];
    y_2nd.ch[3] = y_2nd.ch[1];

    *res = vadd(y_2nd, y_1st);//ch[0,1] low; ch[2,3] h
}

//compensation
static inline void cross2b_to_4b_process(vfloat *x, band4_t *res, cross2b_to_4b_coef_t *coef, cross2b_to_4b_states_t *st){
  //phase compensation  
    vfloat y_c = vmac(coef->k2_c, *x, st->xh1_c);

    st->xh1_c = vmsub(coef->k1_c, y_c, st->xh2_c);
    st->xh1_c = vmac(coef->k1_c, *x, st->xh1_c);

    st->xh2_c = vmsub(coef->k2_c, y_c, *x);
    
    *x = y_c;
/// cross2b_to_4b_process
/*******************************************************/
    *x = vmul(*x ,coef->half);

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
    f = vneg(y_1st);
    y_1st.ch[4] = f.ch[0];
    y_1st.ch[5] = f.ch[1];
    y_1st.ch[6] = f.ch[2];
    y_1st.ch[7] = f.ch[3];
    y_2nd.ch[4] = y_2nd.ch[0];
    y_2nd.ch[5] = y_2nd.ch[1];
    y_2nd.ch[6] = y_2nd.ch[2];
    y_2nd.ch[7] = y_2nd.ch[3];
    
    *res = vadd(y_2nd, y_1st);//ch[0,1] low_low; ch[2,3] high_low ch[4,5] low_high ch[4,5] high_high
}

#endif

#include "apf.h"
 
inline void apf_1st_process(vfloat *x, apf_1st_coef_t *coef, apf_1st_states_t *st){
    //first order
    vfloat xh = vmsub(coef->k, st->xh, *x);
    *x        = vmac(coef->k, xh, st->xh);

    st->xh = xh;
}

inline void apf_2nd_process(vfloat *x, apf_2nd_coef_t *coef, apf_2nd_states_t *st){
    vfloat y_2nd = vmac(coef->k2, *x, st->xh1);

    st->xh1 = vmac(st->s_1[1], coef->k1_2, x);
    st->xh1 = vmsub(st->s_1[0], coef->k1_2, y_2nd);

    st-xh2 = vmsub(x, coef->k2_2, y_2nd);
}

#include "apf.h"
 
inline void apf_1st_process(vfloat *x, apf_1st_coef_t *coef, apf_1st_states_t *st){
    //first order
    vfloat xh = vmsub(coef->k, st->xh, *x);
    *x        = vmac(coef->k, xh, st->xh);

    st->xh = xh;
}
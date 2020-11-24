#include "biquad.h"
 
void biquad_process(vfloat *x, biquad_coef_t *coef, biquad_states_t *st){
    vfloat y;
    if(coef->Enable != 0)
    {
        y = vmac(coef->b0, *x, st->x1);

        st->x1 = vmac(coef->b1, *x, st->x2);
        st->x1 = vmsub(coef->a1, y, st->x1);

        st->x2 = vmul(coef->b2, *x);
        st->x2 = vmsub(y, coef->a2, st->x2);

        *x = y;
    }
}

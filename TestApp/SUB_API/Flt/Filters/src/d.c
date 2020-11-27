#include "biquad.h"

static inline void biquad_process(vfloat *x, biquad_coef_t *coef, biquad_states_t *st){
    vfloat y;
    y = vmac(coef->b0, *x, st->x1);         //10раз по 2
                                        
    st->x1 = vmac(coef->b1, *x, st->x2);
    st->x1 = vmac(coef->a1,  y, st->x1);
                                            //3раза по 8 шт 
    st->x2 = vmul(coef->b2, *x);
    st->x2 = vmac(coef->a2,  y, st->x2);

    *x = y;
}

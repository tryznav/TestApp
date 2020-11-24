#include "eq.h"
 
void eq_process(vfloat *x, eq_coef_t *coef, eq_states_t *st){
    if(coef->Enable != 0){
        for(size_t i = 0; i < EQ_BAND; i++){   
            biquad_process(x, &coef->biquad_coef[i], &st->biquad_states[i]);
        }
    }
}

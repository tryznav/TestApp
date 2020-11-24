#include "eq.h"

void eq_calculate_coef(eq_prm_t *prm,  eq_coef_t *coef){
    for (int32_t i = 0; i < EQ_BAND; i++) 
    {
        biquad_calculate_coef(&prm->biquad_prm[i], &coef->biquad_coef[i]);
    }
}

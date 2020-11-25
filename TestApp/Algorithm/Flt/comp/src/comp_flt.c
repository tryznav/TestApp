#include "comp_flt.h"


static inline void smooth(vfloat *env, vfloat *prev, vfloat *ar, vfloat *ar_m, vfloat *x);

void compressor_process(vfloat *x, comp_states_t *st, comp_coef_t *coef){
//     vfloat x_abs = vabs(*x);

//     vfloat env;
//     vfloat gain_tmp;

//     vfloat ar;
//     vfloat ar_m;

//     smooth(&env, &st->env, &ar, &ar_m, &x_abs);
//     st->env = env;

//     //Calculate gain computer
//     if (st->env < coef->threshold)
//     {
//         gain_tmp = 1;
//     }
//     else
//     {
//         gain_tmp  = (coef->threshold * powf((st->env / coef->threshold), (1.0f / coef->ratio))) / st->env;
//     }

//     //Calculate
    
// //
//     smooth(&env, &st->g_s, &ar, &ar_m, &gain_tmp);
//     st->g_s = env;

//     //Make up gain calculate
//     gain_tmp = vmul(st->g_s, coef->makeUpGain);

//     return  vmul(*x, gain_tmp);
}
 
static inline void smooth(vfloat *env, vfloat *prev, vfloat *ar, vfloat *ar_m, vfloat *x){
    *env = vmul(*prev, *ar);
    *env = vmac(*ar_m, *x, *env);
}
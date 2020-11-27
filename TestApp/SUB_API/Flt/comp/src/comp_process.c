#include "comp.h"

#if CH == 8
void compressor_process(vfloat *x, comp_coef_t *coef, comp_states_t *st){
    vfloat x_abs = vabs(*x);
    // printf("cd\n");
    
    vfloat ONE;
    for (size_t i = 0; i < CH; i++)
    {
        ONE.ch[i] = 1.0f;
    }

    vfloat env;
    vfloat gain_tmp;

    vfloat ar;
    vfloat ar_m;

    vfloat mask = vcpm_gt(x_abs, st->env);

    ar = vblend(coef->e_release, coef->e_attack, mask);
    ar_m = vblend(coef->e_release_m, coef->e_attack_m, mask);
    
    smooth(&env, &st->env, &ar, &ar_m, &x_abs);

    st->env = env;

    mask = vcpm_gt(env, coef->threshold);

    gain_tmp = vdiv(vmul(coef->threshold, vpow(vdiv(st->env, coef->threshold), vdiv(ONE, coef->ratio))), st->env);

    gain_tmp = vblend(ONE, gain_tmp, mask);

// *x = env;

    mask = vcpm_gt(st->g_s, gain_tmp);

    ar = vblend(coef->a_release, coef->a_attack, mask);
    ar_m = vblend(coef->a_release_m, coef->a_attack_m, mask);

    smooth(&env, &st->g_s, &ar, &ar_m, &gain_tmp);
    st->g_s = env;
    

    // //Make up gain calculate
    gain_tmp = vmul(st->g_s, coef->makeUpGain);

    *x = vmul(*x, gain_tmp);
    
}
#endif
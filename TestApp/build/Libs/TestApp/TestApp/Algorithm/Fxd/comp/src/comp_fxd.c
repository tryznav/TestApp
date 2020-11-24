#include "comp_fxd.h"

#define ONE 0x7FFFFFFF

audio_type compressor_fxd(audio_type x, comp_fxd_states_t *st, comp_fxd_coef_t *coef){
    audio_type x_abs = fxd_abs(x);
    audio_type env = 0;
    /* _________________________Envelope detector__________________*/
    if (x_abs > st->env_prev)              // comparison of current gain and previos gain
    {
        // st->env = coef->e_attack * st->env_prev + (1.0f - coef->e_attack) * x_abs;     // if current gain higher than previous -> attac
        st->env = fxd_mul(coef->e_attack, st->env_prev);
        st->env = fxd_add(st->env, fxd_mul(fxd_sub(ONE, coef->e_attack), x_abs));
    }
    else
    {
        //st->env = coef->e_release * st->env_prev + (1.0f - coef->e_release) * x_abs;    // attenuate
        st->env = fxd_mul(coef->e_release, st->env_prev);
        st->env = fxd_add(st->env, fxd_mul(fxd_sub(ONE, coef->e_release), x_abs));
    }

    st->env_prev = st->env;

    /* _________________________Gain computer__________________*/
    if (st->env < coef->threshold)
    {
        // st->x_sc = st->env;
        st->g_c = ONE;
    }
    else
    {
        //st->g_c  = (coef->threshold * powf((st->env / coef->threshold), (1.0f / coef->ratio))) / st->env;
        st->g_c = fxd_div(coef->threshold, st->env);
        st->g_c = fxd_pow(st->g_c, coef->ratio);

    }
        // // printf(" st->g_c %f  coef->ratio%f coef->threshold %f\t",  st->g_c,  coef->ratio, coef->threshold);
    
    /* _________________________Gain smoothing__________________ */
    if (st->g_c <= st->g_sPrev)
    {   
        //ATAC
        // st->g_s = coef->a_attack * st->g_sPrev + (1.0f - coef->a_attack) * st->g_c;
        st->g_s = fxd_mul(coef->a_attack, st->g_sPrev);
        st->g_s = fxd_add(st->g_s, fxd_mul(fxd_sub(ONE, coef->a_attack), st->g_c));
    }
    else
    {
        //RELISE
        // st->g_s = coef->a_release* st->g_sPrev + (1.0f - coef->a_release) * st->g_c;
        st->g_s = fxd_mul(coef->a_release, st->g_sPrev);
        st->g_s = fxd_add(st->g_s, fxd_mul(fxd_sub(ONE, coef->a_release), st->g_c));
    }

        st->g_sPrev = st->g_s;
        st->g_m = (audio_type)saturation( fxd63_rshift(fxd_fmul(st->g_s, coef->makeUpGain), GAIN_FR)) ;
        //;

    return   fxd_mul(x, st->g_m);
}

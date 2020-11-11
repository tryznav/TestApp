#include "comp_flt.h"

audio_type compressor(audio_type x, comp_flt_coef_t *coef, comp_flt_states_t *st){
    audio_type x_abs = 0.0f;
    x_abs = fabsf(st->x);

    if (x_abs > st->env_prev)              // comparison of current gain and previos gain
    {
        st->env = coef->e_attack * st->env_prev + (1.0 - coef->e_attack) * x_abs;     // if current gain higher than previous -> attac
    }
    else
    {
        st->env = coef->e_release * st->env_prev + (1.0 - coef->e_release) * x_abs;    // attenuate
    }

    st->env_prev = st->env;

    if (st->env < coef->threshold)
    {
        st->x_sc = st->env;
        st->g_c = 1;
    }
    else
    {
        st->x_sc = coef->threshold * powf((st->env / coef->threshold), (1.0 / coef->ratio));
        st->g_c  = st->x_sc / st->env;
    }

    if (st->g_c <= st->g_sPrev)
    {
        st->g_s = coef->a_attack * st->g_sPrev + (1.0 - coef->a_attack) * st->g_c;
    }
    else
    {
        st->g_s = coef->a_release* st->g_sPrev + (1.0 - coef->a_release) * st->g_c;
    }

    st->g_sPrev = st->g_s;
    st->g_m = st->g_s * coef->makeUpGain;

    return  (st->x * st->g_m);
}
 
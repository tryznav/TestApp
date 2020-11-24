#include "comp_flt.h"

vfloat compressor(vfloat x, comp_flt_states_t *st, comp_flt_coef_t *coef){
    // printf("here");
    vfloat x_abs = vabs(x);
    vfloat env;

    if (x_abs > st->env_prev)              // comparison of current gain and previos gain
    {
        st->env = coef->e_attack * st->env_prev + (1.0f - coef->e_attack) * x_abs;     // if current gain higher than previous -> attac
    }
    else
    {
        st->env = coef->e_release * st->env_prev + (1.0f - coef->e_release) * x_abs;    // attenuate
    }

    st->env_prev = st->env;
    // printf("st->env %f %f\t", st->env, coef->e_release);

    if (st->env < coef->threshold)
    {
        // st->x_sc = st->env;
        st->g_c = 1;
    }
    else
    {
        st->g_c  = (coef->threshold * powf((st->env / coef->threshold), (1.0f / coef->ratio))) / st->env;
    }
        // // printf(" st->g_c %f  coef->ratio%f coef->threshold %f\t",  st->g_c,  coef->ratio, coef->threshold);
    if (st->g_c <= st->g_sPrev)
    {   
        //ATAC
        st->g_s = coef->a_attack * st->g_sPrev + (1.0f - coef->a_attack) * st->g_c;
    }
    else
    {
        //RELISE
        st->g_s = coef->a_release* st->g_sPrev + (1.0f - coef->a_release) * st->g_c;
    }

        st->g_sPrev = st->g_s;
        st->g_m = st->g_s * coef->makeUpGain;
// printf("g_s %f, \n", st->g_m);

    return  x  * st->g_m;
}
 
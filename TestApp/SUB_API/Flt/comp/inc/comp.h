#ifndef __COMP_H__
#define __COMP_H__

#include "colors.h"
#include "flt_arithmetic.h"
#include "params_id.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


#define ORDER           2
#define NUM_OF_FREQ     3
#define NUM_OF_BAND     (NUM_OF_FREQ + 1)

typedef struct comp_prm_s{
    double threshold;
    double ratio;
    double tauAttack;
    double tauRelease;
    double makeUpGain;
    double samplerate;

    double tauEnvAtt;
    double tauEnvRel;
    int32_t Enable;
    /*float kneeWidth;*/
}comp_prm_t;

typedef struct comp_coef_s{
    vfloat threshold;
    vfloat ratio;

    vfloat      a_attack;//alpha attack
    vfloat      a_release;//alpha relise
    vfloat      a_attack_m;//alpha attack
    vfloat      a_release_m;//alpha relise

    vfloat      e_attack; //env attack
    vfloat      e_release; //env rel
    vfloat      e_attack_m; //env attack
    vfloat      e_release_m; //env rel    
    vfloat      makeUpGain;
    int32_t     Enable;
}comp_coef_t;


typedef struct comp_states_s{
    vfloat x;   //
    vfloat y;

    vfloat x_dB; //
    vfloat x_sc;       //

    vfloat g_s;

    vfloat g_m;

    vfloat g_lin;

    vfloat env;     //
}comp_states_t;

int32_t comp_calculate_coef(comp_prm_t *prm, comp_coef_t *coef);
vfloat compressor(vfloat x, comp_states_t *st, comp_coef_t *coef);
int32_t comp_set_state(comp_states_t *st);
int32_t comp_set_prm_d(comp_prm_t* prm, double samle_rate);
int32_t comp_set_prm_c(comp_prm_t* prm, uint8_t Id, float value);



static inline void smooth(vfloat *env, vfloat *prev, vfloat *ar, vfloat *ar_m, vfloat *x){
    *env = vmul(*prev, *ar);
    *env = vmac(*ar_m, *x, *env);
}

// #if CH == 8
// void compressor_process(vfloat *x, comp_coef_t *coef, comp_states_t *st){
//     vfloat x_abs = vabs(*x);
//     // printf("cd\n");
    
//     vfloat ONE;
//     for (size_t i = 0; i < CH; i++)
//     {
//         ONE.ch[i] = 1.0f;
//     }

//     vfloat env;
//     vfloat gain_tmp;

//     vfloat ar;
//     vfloat ar_m;

//     vfloat mask = vcpm_gt(x_abs, st->env);

//     ar = vblend(coef->e_release, coef->e_attack, mask);
//     ar_m = vblend(coef->e_release_m, coef->e_attack_m, mask);
    
//     smooth(&env, &st->env, &ar, &ar_m, &x_abs);

//     st->env = env;

//     mask = vcpm_gt(env, coef->threshold);

//     gain_tmp = vdiv(vmul(coef->threshold, vpow(vdiv(st->env, coef->threshold), vdiv(ONE, coef->ratio))), st->env);

//     gain_tmp = vblend(ONE, gain_tmp, mask);

// // *x = env;
//     mask = vcpm_gt(st->g_s, gain_tmp);// vcpm_le(, st->g_s);

//     ar = vblend(coef->a_release, coef->a_attack, mask);
//     ar_m = vblend(coef->a_release_m, coef->a_attack_m, mask);

//     // ar = vblend(coef->e_release, coef->e_attack, mask);
//     // ar_m = vblend(coef->e_release_m, coef->e_attack_m, mask);

//     smooth(&env, &st->g_s, &ar, &ar_m, &gain_tmp);
//     st->g_s = env;
    

//     // // //Make up gain calculate
//     gain_tmp = vmul(st->g_s, coef->makeUpGain);

//     *x = vmul(*x, gain_tmp);
    
// }
// #endif
void compressor_process(vfloat *x, comp_coef_t *coef, comp_states_t *st);

#endif

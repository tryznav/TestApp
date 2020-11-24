#ifndef __CHAIN_API_H__
#define __CHAIN_API_H__

#include "params_id.h"
#include "colors.h"

#include "effect_control.h"
#include "effect_process.h"

// #include "comp_flt.h"
#include "cross.h"
#include "eq.h"

// #pragma pack(push,1)
// typedef struct chanes_s{
//     audio_type Left;
//     audio_type Right;
// }chanes_t;
// #pragma pack(pop)

typedef struct chain_prm_s{
    eq_prm_t        eq1;
    int32_t         eq1_enable;
    cross_prm_t     cross;
    int32_t         cross_enable;
    // comp_prm_t      comp_1b;
    // int32_t         comp_1b_enable;
    // comp_prm_t      comp_2b;
    // int32_t         comp_2b_enable;
    // comp_prm_t      comp_3b;
    // int32_t         comp_3b_enable;
    // comp_prm_t      comp_4b;
    // int32_t         comp_4b_enable;
    // eq_prm_t        eq2;
    // int32_t         eq2_enable;
    // comp_prm_t      limiter;
    // int32_t         limiter_enable;
}chain_prm_t;

typedef struct chain_states_s{
    eq_states_t     eq1;
    cross4b_states_t    cross;
    // comp_flt_states_t   comp_1b;
    // comp_flt_states_t   comp_2b;
    // comp_flt_states_t   comp_3b;
    // comp_flt_states_t   comp_4b;
    // eq_flt_states_t     eq2;
    // comp_flt_states_t   limiter;
}chain_states_t;


typedef struct chain_coef_s{
    eq_coef_t       eq1;
    int32_t             eq1_enable;
    cross4b_coef_t      cross;
    int32_t             cross_enable;
    // comp_flt_coef_t     comp_1b;
    // int32_t             comp_1b_enable;
    // comp_flt_coef_t     comp_2b;
    // int32_t             comp_2b_enable;
    // comp_flt_coef_t     comp_3b;
    // int32_t             comp_3b_enable;
    // comp_flt_coef_t     comp_4b;
    // int32_t             comp_4b_enable;
    // eq_flt_coef_t       eq2;
    // int32_t             eq2_enable;
    // comp_flt_coef_t     limiter;
    // int32_t             limiter_enable;
}chain_coef_t;

#endif

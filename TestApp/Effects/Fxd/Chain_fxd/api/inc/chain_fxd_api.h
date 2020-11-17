#ifndef __CHAIN_FXD_API_H__
#define __CHAIN_FXD_API_H__

#include "params_id.h"
#include "colors.h"

#include "chain_fxd_control.h"
#include "chain_fxd_process.h"


#include "cross_fxd.h"
// #include "comp_flt.h"
// #include "eq_flt.h"

// #pragma pack(push,1)
// typedef struct chanes_s{
//     audio_type Left;
//     audio_type Right;
// }chanes_t;
// #pragma pack(pop)

typedef struct chain_fxf_prm_s{
    // eq_prm_t        eq1;
    // int32_t         eq1_enable;
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
}chain_flt_prm_t;

typedef struct chain_flt_states_s{
    // eq_flt_states_t     eq1;
    cross4b_fxd_states_t    cross;
    // comp_flt_states_t   comp_1b;
    // comp_flt_states_t   comp_2b;
    // comp_flt_states_t   comp_3b;
    // comp_flt_states_t   comp_4b;
    // eq_flt_states_t     eq2;
    // comp_flt_states_t   limiter;
}chain_flt_states_t;

typedef struct states_s{
    chain_flt_states_t Left;
    chain_flt_states_t Right;
}states_t;

typedef struct chain_flt_coef_s{
    // eq_flt_coef_t       eq1;
    // int32_t             eq1_enable;
    cross_fxd_coef_t    cross[3];
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
}chain_flt_coef_t;

#endif

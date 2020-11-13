#ifndef __CHAIN_FLT_API_H__
#define __CHAIN_FLT_API_H__

#include "params_id.h"
#include "colors.h"

#include "chain_flt_control.h"
#include "chain_flt_process.h"

#include "comp_flt.h"
#include "cross_flt.h"
#include "eq_flt.h"

// #pragma pack(push,1)
// typedef struct chanes_s{
//     audio_type Left;
//     audio_type Right;
// }chanes_t;
// #pragma pack(pop)

typedef struct chain_flt_prm_s{
    eq_prm_t        eq;
    int32_t         eq_enable;
    cross_prm_t     cross;
    int32_t         cross_enable;
    comp_prm_t      comp;
    int32_t         comp_enable;
}chain_flt_prm_t;

typedef struct chain_flt_states_s{
    eq_flt_states_t     eq;
    cross4b_states_t    cross;
    comp_flt_states_t   comp;
}chain_flt_states_t;

typedef struct states_s{
    chain_flt_states_t Left;
    chain_flt_states_t Right;
}states_t;

typedef struct chain_flt_coef_s{
    eq_flt_coef_t       eq;
    int32_t             eq_enable;
    cross_flt_coef_t    cross;
    int32_t             cross_enable;
    comp_flt_coef_t     comp;
    int32_t             comp_enable;
}chain_flt_coef_t;

#endif

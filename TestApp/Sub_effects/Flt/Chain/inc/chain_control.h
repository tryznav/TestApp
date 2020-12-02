#ifndef __CHAIN_CONTROL_H__
#define __CHAIN_CONTROL_H__

#include "chain.h"

#include "eq_control.h"
#include "compressors_control.h"

#include <stdint.h>
#include <stddef.h>

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void chain_control_get_sizes(
    size_t*                 params_bytes,
    size_t*                 coeffs_bytes);

void chain_control_initialize(
    chain_params_t*         params,
    chain_coeffs_t*         coeffs,
    uint32_t                sample_rate);

int32_t chain_set_parameter(
    void*                   params,
    int32_t                 id,
    float                   value);

void chain_update_coeffs(
    chain_params_t const*  params,
    chain_coeffs_t*        coeffs);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif
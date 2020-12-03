#ifndef __EQ_CONTROL_H__
#define __EQ_CONTROL_H__

#include "eq.h"
#include "filters_control.h"
#include "set_params.h"

#include <stdint.h>
#include <stddef.h>

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void eq_control_get_sizes(
    size_t*             params_bytes,
    size_t*             coeffs_bytes);

void eq_control_initialize(
    eq_params_t*        params,
    eq_coeffs_t*        coeffs,
    uint32_t            sample_rate);

int32_t eq_set_parameter(
    void*               params,
    int32_t             id,
    float               value);

void eq_update_coeffs(
    eq_params_t const*  params,
    eq_coeffs_t*        coeffs);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif
#ifndef __FILTERS_CONTROL_H__
#define __FILTERS_CONTROL_H__

#include <stdint.h>
#include <stddef.h>

#include "filters.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void biquad_control_get_sizes(
    size_t*                         params_bytes,
    size_t*                         coeffs_bytes);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void biquad_control_initialize(
    biquad_params_t*                params,
    biquad_coeffs_t*                coeffs,
    uint32_t                        sample_rate);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int32_t biquad_set_parameter(
    void*                           params,
    int32_t                         id,
    float                           value);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void biquad_update_coeffs(
    biquad_params_t const*          params,
    biquad_coeffs_t*                coeffs);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void biquad_cascade2_control_get_sizes(
    size_t*                         params_bytes,
    size_t*                         coeffs_bytes);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void biquad_cascade2_control_initialize(
    biquad_cascade2_params_t*       params,
    biquad_cascade2_coeffs_t*       coeffs,
    uint32_t                        sample_rate);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int32_t biquad_cascade2_set_parameter(
    void*       params,
    int32_t     id,
    float       value);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void biquad_cascade2_update_coeffs(
    biquad_cascade2_params_t const* params,
    biquad_cascade2_coeffs_t*       coeffs);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void biquad_cascade4_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void biquad_cascade4_control_initialize(
    biquad_cascade4_params_t*       params,
    biquad_cascade4_coeffs_t*       coeffs,
    uint32_t                        sample_rate);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int32_t biquad_cascade4_set_parameter(
    void*                           params,
    int32_t                         id,
    float                           value);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void biquad_cascade4_update_coeffs(
    biquad_cascade4_params_t const* params,
    biquad_cascade4_coeffs_t*       coeffs);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/******************************************************************************/
void apf2_band2_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes);

void apf2_band2_control_initialize(
    apf2_band2_params_t*       params,
    apf2_band2_coeffs_t*       coeffs,
    uint32_t                        sample_rate);

int32_t apf2_band2_set_parameter(
    void*       params,
    int32_t     id,
    float       value);

void apf2_band2_update_coeffs(
    apf2_band2_params_t const* params,
    apf2_band2_coeffs_t*       coeffs);

/******************************************************************************/
#endif
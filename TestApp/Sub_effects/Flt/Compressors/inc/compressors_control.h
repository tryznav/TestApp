#ifndef __COMPRESSORS_CONTROL_H__
#define __COMPRESSORS_CONTROL_H__

#include "crossovers_control.h"

#include <stdint.h>
#include <stddef.h>

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "compressors.h"
#include "compressors_control.h"

/*******************************************************************************/
void compressor_4band_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes);

void compressor_4band_control_initialize(
    compressor_4band_params_t*         params,
    compressor_4band_coeffs_t*         coeffs,
    uint32_t                           sample_rate);

int32_t compressor_4band_set_parameter(
    void*       params,
    int32_t     id,
    float       value);

void compressor_4band_update_coeffs(
    compressor_4band_params_t const* params,
    compressor_4band_coeffs_t*       coeffs);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif
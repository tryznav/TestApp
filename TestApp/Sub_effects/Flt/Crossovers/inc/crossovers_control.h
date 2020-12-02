#ifndef __CROSSOVERS_CONTROL_H__
#define __CROSSOVERS_CONTROL_H__

#include <stdint.h>
#include <stddef.h>

#include "crossovers.h"
#include "filters_control.h"

void crossover2b_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes);

void crossover2b_control_initialize(
    crossover2b_params_t*       params,
    crossover2b_coeffs_t*       coeffs,
    uint32_t                    sample_rate);

int32_t crossover2b_set_parameter(
    void*       params,
    int32_t     id,
    float       value);

void crossover2b_update_coeffs(
    crossover2b_params_t const* params,
    crossover2b_coeffs_t*       coeffs);

/**************************************************/
void crossover2b_to_4b_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes);

void crossover2b_to_4b_control_initialize(
    crossover2b_to_4b_params_t*       params,
    crossover2b_to_4b_coeffs_t*       coeffs,
    uint32_t                    sample_rate);

int32_t crossover2b_to_4b_set_parameter(
    void*       params,
    int32_t     id,
    float       value);

void crossover2b_to_4b_update_coeffs(
    crossover2b_to_4b_params_t const* params,
    crossover2b_to_4b_coeffs_t*       coeffs);

/**************************************************/
void crossover4b_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes);

void crossover4b_control_initialize(
    crossover4b_params_t*       params,
    crossover4b_coeffs_t*       coeffs,
    uint32_t                    sample_rate);

int32_t crossover4b_set_parameter(
    void*       params,
    int32_t     id,
    float       value);

void crossover4b_update_coeffs(
    crossover4b_params_t const* params,
    crossover4b_coeffs_t*       coeffs);

/**************************************************/
#endif
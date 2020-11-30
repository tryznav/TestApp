#ifndef __CHAIN_PROCESS_H__
#define __CHAIN_PROCESS_H__

#include <stdint.h>
#include <stddef.h>

#include "chain.h"
#include "eq_process.h"


/******************************************************************************/

void chain_process_get_sizes(
    size_t*                 states_bytes);

void chain_reset(
    eq_coeffs_t*            coeffs,
    eq_states_t*            states);

static inline void chain_process(
    chain_coeffs_t const*      coeffs,
    chain_states_t*            states,
    stereo_t*               frame){ //frame
    eq_process((eq_coeffs_t const*)&coeffs->eq1, &states->eq1, frame);
}

#endif
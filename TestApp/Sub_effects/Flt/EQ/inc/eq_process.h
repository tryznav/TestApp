#ifndef __EQ_PROCESS_H__
#define __EQ_PROCESS_H__

#include <stdint.h>
#include <stddef.h>

#include "eq.h"
#include "filters_process.h"


#include "filters.h"
#include <stdlib.h>


/******************************************************************************/

void eq_process_get_sizes(
    size_t*                 states_bytes);

void eq_reset(
    eq_coeffs_t const*      coeffs,
    eq_states_t*            states);

static inline void eq_process(
    eq_coeffs_t const*      coeffs,
    eq_states_t*            states,
    stereo_t*               frame){ //frame 
    for(uint32_t i = 0; i < 2; i++){
        biquad_cascade4_process((biquad_cascade4_coeffs_t const *)&coeffs->cascade4[i], (biquad_cascade4_states_t *)&states->cascade4[i], frame);
    }
    biquad_cascade2_process((biquad_cascade2_coeffs_t const *)&coeffs->cascade2, &states->cascade2, frame);
}

#endif
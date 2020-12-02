#include "filters.h"
#include "eq_process.h"

/******************************************************************************/

void eq_process_get_sizes(
    size_t*     states_bytes){
    *states_bytes = sizeof(eq_states_t);
}

/*******************************************************************************/

void eq_reset(
    eq_coeffs_t const*  coeffs,
    eq_states_t*        states){

    for(uint32_t i = 0; i < 2; i++){
        biquad_cascade4_reset((biquad_cascade4_coeffs_t const*)&coeffs->cascade4[i], &states->cascade4[i]);
    }
    biquad_cascade2_reset((biquad_cascade2_coeffs_t const*)&coeffs->cascade2, &states->cascade2);
}

/******************************************************************************/

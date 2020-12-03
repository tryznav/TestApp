#include "compressor_4band_process.h"

/******************************************************************************/

void compressor_4band_process_get_sizes(
    size_t*     states_bytes){
    *states_bytes = sizeof(compressor_4band_states_t);
}


void compressor_reset(
    compressor_coeffs_t const*   coeffs,
    compressor_states_t*         states){
    eq_reset((eq_coeffs_t const*)&coeffs->eq1, &states->eq1);
    crossover4b_reset((crossover4b_coeffs_t const*)&coeffs->crossover, &states->crossover);
    states->crossover

}

/******************************************************************************/
#include "chain_process.h"

/******************************************************************************/

void chain_process_get_sizes(
    size_t*     states_bytes){
    *states_bytes = sizeof(chain_states_t);
}


void chain_reset(
    chain_coeffs_t const*   coeffs,
    chain_states_t*         states){
    eq_reset((eq_coeffs_t const*)&coeffs->eq1, &states->eq1);
    compressor_4band_reset((compressor_4band_coeffs_t const*)&coeffs->compressor, &states->compressor);

}

/******************************************************************************/
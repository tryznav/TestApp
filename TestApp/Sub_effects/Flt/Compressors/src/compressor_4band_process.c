#include "compressors_process.h"

/******************************************************************************/

void compressor_4band_process_get_sizes(
    size_t*     states_bytes){
    *states_bytes = sizeof(compressor_4band_states_t);
}

void compressor_4band_reset(
    compressor_4band_coeffs_t const*   coeffs,
    compressor_4band_states_t*         states){
    crossover4b_reset((crossover4b_coeffs_t const*)&coeffs->crossover, &states->crossover);
    for(uint32_t i = 0; i < EIGHT; i++){
        states->g_s.ch[i] = 1.0f;
        states->env.ch[i] = 0.0f;
    }
}

/******************************************************************************/
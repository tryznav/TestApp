#include "Filters.h"
#include <stdlib.h>

#ifdef WIN32
    #include <Windows.h>
#else
    // #include <unistd.h>
#endif

/******************************************************************************/

int32_t biquad_cascade_process_get_sizes(
    size_t*     states_bytes){

    *states_bytes = sizeof(chain_states_t);

    return 0;
}

/*******************************************************************************/

int32_t biquad_cascade_reset(
    void const* coeffs,
    void*       states){

    chain_states_t *st = (chain_states_t *)states;

    return 0;
}

/******************************************************************************/
static int32_t check(void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count)
{
    if(audio == NULL){
        fprintf(stderr, RED "Error:\t"RESET BOLDWHITE"Wrong 'audio' pointer. Rejected\n"RESET);
        return -1;
    }

    if(states == NULL){
        fprintf(stderr, RED "Error:\t"RESET BOLDWHITE"Wrong 'states' pointer. Rejected\n"RESET);
        return -1;
    }

    if(coeffs == NULL){
        fprintf(stderr, RED "Error:\t"RESET BOLDWHITE"Wrong 'coeffs' pointer. Rejected\n"RESET);
        return -1;
    }
    
    if(samples_count == 0){
        fprintf(stderr, RED "Error:\t"RESET BOLDWHITE"Wrong 'coeffs' pointer. Rejected\n"RESET);
        return -1;
    }

    return 0;
}

static inline void chain_flt(vfloat *x, chain_states_t *st, chain_coef_t *coef){
    for(int i = 0; i < CH; i++){
        x->ch[i] = x->ch[i] * 2;
    }
}

int32_t biquad_cascade_process(
    void const* coeffs,
    void*       states,
    void*       audio){

    for(uint32_t i = 0; i < FRAME_SIZE; i++){

    }

    return 0;
}



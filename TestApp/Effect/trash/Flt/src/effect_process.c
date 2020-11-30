#include "chain_process.h"

/******************************************************************************/

int32_t effect_process_get_sizes(
    size_t*     states_bytes){

    *states_bytes = sizeof(chain_states_t);

    return 0;
}

/*******************************************************************************/

int32_t effect_reset(
    void const* coeffs,
    void*       states){
    
    chain_reset((chain_coeffs_t const *)&coeffs, (chain_states_t *)states);
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

int32_t effect_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count){

    int n = check(coeffs, states, audio, samples_count);
    if(n == -1){
        return -1;
    }
    chain_coef_t const* _coeffs = (chain_coef_t const*) coeffs;
    chain_states_t*     _states = (chain_states_t*)     states;
    stereo_t *          _audio  = (stereo_t *)audio;

    for(uint32_t a_index = 0; a_index < samples_count; a_index += FRAME_SIZE){
        chain_process(_coeffs, _states, _audio[a_index]);
    }

    return 0;
}

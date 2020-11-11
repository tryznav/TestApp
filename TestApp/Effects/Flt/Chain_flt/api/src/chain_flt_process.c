#include "chain_flt_api.h"
#include <stdlib.h>

#ifdef WIN32
    #include <Windows.h>
#else
    // #include <unistd.h>
#endif

/******************************************************************************/

int32_t apf_flt_process_get_sizes(
    size_t*     states_bytes){

    *states_bytes = sizeof(states_t);

    return 0;
}

/*******************************************************************************/

int32_t apf_flt_reset(
    void const* coeffs,
    void*       states){

    states_t *st = (states_t *)states;
    comp_flt_set_state(&st->Left.comp);
    cross_flt_set_state(&st->Left.cross);
    eq_flt_set_state(&st->Left.eq);
    comp_flt_set_state(&st->Right.comp);
    cross_flt_set_state(&st->Right.cross);
    eq_flt_set_state(&st->Right.eq);
    
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

int32_t apf_flt_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count){
    int n = check(coeffs, states, audio, samples_count);
    if(n == -1){
        return -1;
    }

    chanes_t *_audio = (chanes_t *)(audio);
    states_t* st = (states_t*)states;
    chain_flt_coef_t *coef = (chain_flt_coef_t *)coeffs;

    for(uint32_t a_index = 0; a_index < samples_count; a_index++){
        _audio[a_index].Left = compressor(_audio[a_index].Left, &coef->comp, &st->Left.comp);
        // // _audio[a_index].Right = (_audio[a_index].Right +  _audio[a_index].Left)*0.5f;
        // _audio[a_index].Right = (audio_type)coef->apf_dbl((double)_audio[a_index].Left, &(_st->Right), &(coef->apf_coef));
        
    }

    return 0;
}

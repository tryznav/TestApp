#include "apf_api.h"
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

    states_t *_st = (states_t *)states;
    for(int order = (APF_ORDER - 1); order >= 0 ; order--){
        _st->Left.xh[order] = 0.0f;
        _st->Right.xh[order] = 0.0f;
        _st->Right.xh_dbl[order] = 0.0;
        _st->Left.xh_dbl[order] = 0.0;
        _st->Left.y[order] = 0.0f;
        _st->Right.y[order] = 0.0f;
        _st->Left.y_dbl[order] = 0.0;
        _st->Right.y_dbl[order] = 0.0;
    }
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
    states_t* _st = (states_t*)states;
    coef_t* coef = (coef_t*)coeffs;

    for(uint32_t a_index = 0; a_index < samples_count; a_index++){
        _audio[a_index].Left = coef->apf(_audio[a_index].Left, &(_st->Left), &(coef->apf_coef));
        // _audio[a_index].Right = (_audio[a_index].Right +  _audio[a_index].Left)*0.5f;
        _audio[a_index].Right = (audio_type)coef->apf_dbl((double)_audio[a_index].Left, &(_st->Right), &(coef->apf_coef));
    }

    return 0;
}

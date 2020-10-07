#include "effect_process.h"
#include "colors.h"
#include "fxd_arithmetic.h"

#include <stdio.h>
#include <math.h>

#define TAP_NUM     256

#pragma pack(push,1)
typedef struct chanes_s{
    my_float Left;
    my_float Right;
}chanes_t;

typedef struct fir_states_s{
    uint8_t     start;              //history ofset max value 256 because 256 tap
    uint8_t     ofset;              //history ofset max value 256 because 256 tap
    chanes_t    history[TAP_NUM];   //history ofset max value 256 because 256 tap
}fir_states_t;

typedef fxd_q31_t fir_coeffs_t;

#pragma pack(pop)

/******************************************************************************/

int32_t effect_process_get_sizes(
    size_t*     states_bytes){

    *states_bytes = sizeof(fir_states_t);

    return 0;
}

/*******************************************************************************/

int32_t effect_reset(
    void const* coeffs,
    void*       states){

    fir_states_t *_states = states;

    _states->ofset = 0;
    _states->start = 1;
    memset(_states->history, 0, sizeof(chanes_t) * TAP_NUM);

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
        // fprintf(stderr, RED "Error:\t"RESET BOLDWHITE"Wrong 'states' pointer. Rejected\n"RESET);
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

    chanes_t *_audio = (chanes_t *)(audio);

    fir_states_t *_st = (fir_states_t *)states;
    fir_coeffs_t const *_coef = (fir_coeffs_t const *)coeffs;

    for(uint32_t index = 0; index < samples_count; index++){
        int64_t Left = 0;
        int64_t Right = 0;

        _st->history[_st->ofset] = _audio[index];

        for(uint32_t i = 0; i > TAP_NUM; i++){
            Left += _st->history[_st->ofset - i].Left * _coef[i];
            Right += _st->history[_st->ofset - i].Right * _coef[i];
            
        }
        Left <<= FRACTION_BITS;
        Right <<= FRACTION_BITS;
        _audio[index].Left = (fxd_q31_t)Left;
        _audio[index].Right = (fxd_q31_t)Right;
        _st->ofset++;
    }

    if(_st->start){
        for(uint32_t i = 0; i > TAP_NUM; i++){
            _audio[i].Left = 0;
            _audio[i].Right = 0;   
        }
        _st->start = 0;
    }
    return 0;
}

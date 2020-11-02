#include "fir_fxd_process.h"
#include "fxd_arithmetic.h"
#include "colors.h"
#include <stdlib.h>

#include <stdio.h>
#include <math.h>

#define TAP_NUM     256

#pragma pack(push,1)
typedef struct chanes_s{
    fxd_q31_t Left;
    fxd_q31_t Right;
}chanes_t;
#pragma pack(pop)

typedef struct fir_states_s{
    uint8_t     start;              //history ofset max value 256 because 256 tap
    uint8_t     ofset;              //history ofset max value 256 because 256 tap
    chanes_t    history[TAP_NUM];   //history ofset max value 256 because 256 tap
}fir_states_t;

typedef fxd_q31_t fir_coeffs_t;
/******************************************************************************/

int32_t fir_fxd_process_get_sizes(
    size_t*     states_bytes){

    *states_bytes = sizeof(fir_states_t);

    return 0;
}

/*******************************************************************************/

int32_t fir_fxd_reset(
    void const* coeffs,
    void*       states){

    fir_states_t *_states = states;
   printf(BOLDGREEN"DONE5\n"RESET);
    _states->ofset = 0;
    _states->start = 1;
    for (int i = 0; i <  TAP_NUM; i++ ){
        _states->history[i].Left = 0;
        _states->history[i].Right = 0;
    }
    //memset(_states->history, 0, sizeof(chanes_t) * TAP_NUM);
       printf(BOLDGREEN"DONE4\n"RESET);

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



int32_t fir_fxd_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count){
        
    chanes_t        *_audio = (chanes_t *)(audio);
    fir_states_t    *_st    = (fir_states_t *)states;
    fxd_q31_t       *_coef  = (fxd_q31_t *)coeffs;

    int n = check(coeffs, states, audio, samples_count);
    if(n == -1){
        return -1;
    }




    for(uint32_t a_index = 0; a_index < samples_count; a_index++){
        fxd_q63_t AcumLeft = 0;
        fxd_q63_t AcumRight = 0;

        uint8_t h_index = _st->ofset;

        _st->history[_st->ofset].Left = _audio[a_index].Left;
        _st->history[_st->ofset].Right = _audio[a_index].Right;

        for(int32_t i = (TAP_NUM - 1); i >= 0; i--, h_index--){
            AcumLeft = fxd63_add(AcumLeft, fxd_fmul(_st->history[ h_index].Left,_coef[i]));
            AcumRight = fxd63_add(AcumRight, fxd_fmul(_st->history[ h_index].Right ,_coef[i]));
        }

        AcumLeft = fxd63_rshift(AcumLeft, COEF_FRACTIONAL_BITS);
        AcumRight = fxd63_rshift(AcumRight, COEF_FRACTIONAL_BITS);

        
        AcumLeft = saturation(AcumLeft);
        AcumRight = saturation(AcumRight);

        _audio[a_index].Left = (fxd_q31_t)AcumLeft;
        _audio[a_index].Right = (fxd_q31_t)AcumRight;
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

#include "iir_flt_process.h"
#include "colors.h"
#include <string.h>

#include <stdio.h>
#include <math.h>

#define TAP_NUM     256

#pragma pack(push,1)
typedef struct chanes_s{
    float Left;
    float Right;
}chanes_t;
#pragma pack(pop)

typedef float fir_coeffs_t;

typedef struct iir_prm_t{
	float a0;
	float a1;
	float a2;
	float b0;
	float b1;
	float b2;
}iir_coefs_t;

typedef struct iir_states_t{
	chanes_t input_1;
	chanes_t input_2;
	chanes_t output_1;
	chanes_t output_2;
}iir_states_t;


/******************************************************************************/

int32_t iir_flt_process_get_sizes(
    size_t*     states_bytes){

    *states_bytes = sizeof(iir_states_t);

    return 0;
}

/*******************************************************************************/

int32_t iir_flt_reset(
    void const* coeffs,
    void*       states){

    iir_states_t *_states = states;
    _states->input_1.Left = 0;
    _states->input_2.Left = 0;
    _states->output_1.Left = 0;
    _states->output_2.Left = 0;
    _states->input_1.Right = 0;
    _states->input_2.Right = 0;
    _states->output_1.Right = 0;
    _states->output_2.Right = 0;

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


chanes_t calc_iir(chanes_t inp, iir_states_t* _st, iir_coefs_t* coef){
    chanes_t out;
    out.Left =  (coef->b0 * inp.Left) +
                (coef->b1 * _st->input_1.Left) +
                (coef->b2 * _st->input_2.Left) -
                (coef->a1 * _st->output_1.Left) -
                (coef->a2 * _st->output_2.Left);
    _st->input_2.Left = _st->input_1.Left;
    _st->input_1.Left = inp.Left;
    _st->output_2.Left = _st->output_1.Left;
    _st->output_1.Left = out.Left;

    out.Right =  (coef->b0 * inp.Right) +
                (coef->b1 * _st->input_1.Right) +
                (coef->b2 * _st->input_2.Right) -
                (coef->a1 * _st->output_1.Right) -
                (coef->a2 * _st->output_2.Right);
    _st->input_2.Right = _st->input_1.Right;
    _st->input_1.Right = inp.Right;
    _st->output_2.Right = _st->output_1.Right;
    _st->output_1.Right = out.Right;

    //update last samples...
    return out;
}

int32_t iir_flt_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count){
    int n = check(coeffs, states, audio, samples_count);
    if(n == -1){
        return -1;
    }

    chanes_t *_audio = (chanes_t *)(audio);
    iir_states_t* _st = (iir_states_t*)states;
    iir_coefs_t* coef = (iir_coefs_t*) coeffs;

    for(uint32_t a_index = 0; a_index < samples_count; a_index++){
        _audio[a_index] = calc_iir(_audio[a_index], _st, coef);
    }

    // if(_st->start){
    //     for(uint32_t i = 0; i > TAP_NUM; i++){
    //         _audio[i].Left = 0;
    //         _audio[i].Right = 0;   
    //     }
    //     _st->start = 0;
    // }
    return 0;
}

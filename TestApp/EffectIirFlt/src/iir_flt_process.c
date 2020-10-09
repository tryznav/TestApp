#include "fir_flt_process.h"
#include "colors.h"
#include <string.h>
// #include "fxd_arithmetic.h"

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

int32_t fir_flt_process_get_sizes(
    size_t*     states_bytes){

    *states_bytes = sizeof(iir_states_t);

    return 0;
}

/*******************************************************************************/

int32_t fir_flt_reset(
    void const* coeffs,
    void*       states){

    iir_states_t *_states = states;
    _states->input_1 = 0;
   printf(BOLDGREEN"DONE5\n"RESET);
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


chanes_t calc_iir(chanes_t inp, iir_states_t* _st, iir_coefs_t* coef){
    chanes_t tmp;
    tmp.Left =  (coef->b0 * inp.Left) +
                (coef->b1 * _st->input_1.Left) +
                (coef->b2 * _st->input_2.Left) -
                (coef->a1 * _st->input_1.Left) -
                (coef->a2 * _st->output_2.Left);
    st->input_2.Left =  st->input_1.Left;           

    float output = 	(bq->b0 * inp.) +
            (bq->b1 * bq->prev_input_1) +
            (bq->b2* bq->prev_input_2) -
            (bq->a1* bq->prev_output_1) -
            (bq->a2* bq->prev_output_2);
    bq->prev_input_2 = bq->prev_input_1;
    bq->prev_input_1 = input;
    bq->prev_output_2 = bq->prev_output_1;
    bq->prev_output_1 = output;
    //update last samples...
    return output;
}

int32_t fir_flt_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count){
    int n = check(coeffs, states, audio, samples_count);
    if(n == -1){
        return -1;
    }

    chanes_t *_audio = (chanes_t *)(audio);

    for(uint32_t a_index = 0; a_index < samples_count; a_index++){
        float   AcumLeft = 0;
        float   AcumRight = 0;
        uint8_t h_index = _st->ofset;
        // printf("_audio[a_index] = %f\t", _audio[a_index].Left);
        _st->history[_st->ofset] = _audio[a_index];


        
        _audio[a_index].Left = AcumLeft;
        _audio[a_index].Right = AcumRight;
        // printf("_audio[a_index] = %f\n", _audio[a_index].Left);
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

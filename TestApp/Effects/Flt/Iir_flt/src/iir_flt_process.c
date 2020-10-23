#include "iir_flt.h"

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
    _states->input_1.Left = 0.0f;
    _states->input_2.Left = 0.0f;
    _states->output_1.Left = 0.0f;
    _states->output_2.Left = 0.0f;
    _states->input_1.Right = 0.0f;
    _states->input_2.Right = 0.0f;
    _states->output_1.Right = 0.0f;
    _states->output_2.Right = 0.0f;

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


chanes_t calc_iir(chanes_t inp, iir_states_t* _st, iir_coefs_t* coef){
    chanes_t out;
    
    acum_type acum = flt_mul(coef->b0, inp.Left);
    acum = flt_add(acum, flt_mul(coef->b1, _st->input_1.Left));
    acum = flt_add(acum, flt_mul(coef->b2, _st->input_2.Left));
    acum = flt_sub(acum, flt_mul(coef->a1, _st->output_1.Left));
    acum = flt_sub(acum, flt_mul(coef->a2, _st->output_2.Left));
    out.Left = (my_float)acum;

    acum = 0.0f;
    acum = flt_mul(coef->b0, inp.Right);
    acum = flt_add(acum, flt_mul(coef->b1, _st->input_1.Right));
    acum = flt_add(acum, flt_mul(coef->b2, _st->input_2.Right));
    acum = flt_sub(acum, flt_mul(coef->a1, _st->output_1.Right));
    acum = flt_sub(acum, flt_mul(coef->a2, _st->output_2.Right));
    out.Right = (my_float)acum;
out.Right = inp.Right;
    //update last samples...
    _st->input_2.Right = _st->input_1.Right;
    _st->input_1.Right = inp.Right;
    _st->output_2.Right = _st->output_1.Right;
    _st->output_1.Right = out.Right;

    _st->input_2.Left = _st->input_1.Left;
    _st->input_1.Left = inp.Left;
    _st->output_2.Left = _st->output_1.Left;
    _st->output_1.Left = out.Left;

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

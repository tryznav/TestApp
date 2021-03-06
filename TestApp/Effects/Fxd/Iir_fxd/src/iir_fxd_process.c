#include "iir_fxd_process.h"
#include "iir_fxd.h"

/******************************************************************************/

int32_t iir_fxd_process_get_sizes(
    size_t*     states_bytes){

    *states_bytes = sizeof(iir_states_t);

    return 0;
}

/*******************************************************************************/

int32_t iir_fxd_reset(
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
    _states->dizer.Left = 0;
    _states->dizer.Right = 0;
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


static chanes_t calc_iir(chanes_t inp, iir_states_t* _st, iir_coefs_t* coef){
    chanes_t out;

    fxd_q63_t acum = fxd_fmul(coef->b0, inp.Left);
    acum += _st->dizer.Left;
    acum = fxd63_add(acum, fxd_fmul(coef->b1, _st->input_1.Left));
    acum = fxd63_add(acum, fxd_fmul(coef->b2, _st->input_2.Left));
    acum = fxd63_sub(acum, fxd_fmul(coef->a1, _st->output_1.Left));
    acum = fxd63_sub(acum, fxd_fmul(coef->a2, _st->output_2.Left));
    _st->dizer.Left = (fxd_q31_t)(acum & ((1u << COEF_FRACTIONAL_BITS) - 1));
    acum = fxd63_rshift(acum, COEF_FRACTIONAL_BITS);
    acum = saturation(acum);
    out.Left = (fxd_q31_t)acum;


    
    acum = fxd_fmul(coef->b0, inp.Right);
    // acum += _st->dizer.Right;
    acum = fxd63_add(acum, fxd_fmul(coef->b1, _st->input_1.Right));
    acum = fxd63_add(acum, fxd_fmul(coef->b2, _st->input_2.Right));
    acum = fxd63_sub(acum, fxd_fmul(coef->a1, _st->output_1.Right));
    acum = fxd63_sub(acum, fxd_fmul(coef->a2, _st->output_2.Right));
    // printf("_st->dizer.Left = %d\n", _st->dizer.Left);
    // _st->dizer.Right = (fxd_q31_t)(acum & (COEF_FRACTIONAL_BITS - 1));
    acum = fxd63_rshift(acum, COEF_FRACTIONAL_BITS);
    acum = saturation(acum);
    out.Right = (fxd_q31_t)acum;

    //update last samples
    _st->input_2.Left = _st->input_1.Left;
    _st->input_1.Left = inp.Left;
    _st->output_2.Left = _st->output_1.Left;
    _st->output_1.Left = out.Left;

    _st->input_2.Right = _st->input_1.Right;
    _st->input_1.Right = inp.Right;
    _st->output_2.Right = _st->output_1.Right;
    _st->output_1.Right = out.Right;
    //  printf("ALL\n");
    // exit(0);

    return out;
}

int32_t iir_fxd_process(
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

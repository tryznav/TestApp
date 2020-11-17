#include "chain_fxd_api.h"
#include <stdlib.h>

#ifdef WIN32
    #include <Windows.h>
#else
    // #include <unistd.h>
#endif

/******************************************************************************/

int32_t chain_fxd_process_get_sizes(
    size_t*     states_bytes){

    *states_bytes = sizeof(states_t);

    return 0;
}

/*******************************************************************************/

int32_t chain_fxd_reset(
    void const* coeffs,
    void*       states){

    states_t *st = (states_t *)states;
    //    eq_flt_set_state(&st->Left.eq1);
    cross_fxd_set_state(&st->Left.cross);

    // comp_flt_set_state(&st->Left.comp_1b);
    // comp_flt_set_state(&st->Left.comp_1b);
    // comp_flt_set_state(&st->Left.comp_2b);
    // comp_flt_set_state(&st->Left.comp_3b);
    // comp_flt_set_state(&st->Left.comp_4b);
    //   eq_flt_set_state(&st->Left.eq2);
    // comp_flt_set_state(&st->Left.limiter);

    //    eq_flt_set_state(&st->Right.eq1);
    cross_fxd_set_state(&st->Right.cross);

    // comp_flt_set_state(&st->Right.comp_1b);
    // comp_flt_set_state(&st->Right.comp_1b);
    // comp_flt_set_state(&st->Right.comp_2b);
    // comp_flt_set_state(&st->Right.comp_3b);
    // comp_flt_set_state(&st->Right.comp_4b);
    //   eq_flt_set_state(&st->Right.eq2);
    // comp_flt_set_state(&st->Right.limiter);
    printf("chain_flt_reset\n");
    
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

// static audio_type apf_flt_2nd(audio_type x, apf_states_2nd_t *st, apf_coef_2nd_t *coef){
//     audio_type  y = flt_mac(st->xh[0], coef->k2, x);
    
//     st->xh[0] = flt_mac(st->xh[1], coef->k1, x);
//     st->xh[0] = flt_msub(st->xh[0], coef->k1, y);

//     st->xh[1] = flt_msub(x, coef->k2, y);

//     return y;
// }

// static inline audio_type chain_flt(audio_type x, chain_flt_states_t *st, chain_flt_coef_t *coef){
//     audio_type res = eq_flt(x,  &(st->eq1), &(coef->eq1));
//     if (coef->comp_4b_enable)
//     {
//         band4_t b = cross4b_flt(res, &(st->cross), coef->cross);
//         if(coef->comp_1b.Enable){
//             b.low.band1 = compressor(b.low.band1, &(st->comp_1b), &(coef->comp_1b));
//         }
//         if(coef->comp_2b.Enable){
//             b.low.band2 = compressor(b.low.band2, &(st->comp_2b), &(coef->comp_2b));
//         }
//         if(coef->comp_3b.Enable){
//              b.high.band1 = compressor(b.high.band1, &(st->comp_3b), &(coef->comp_3b));
//         }
//         if(coef->comp_3b.Enable){
//             b.high.band2 = compressor(b.high.band2, &(st->comp_4b), &(coef->comp_4b));
//         }
//         res = flt_add( flt_add(b.low.band1, b.low.band2), flt_add(b.high.band1, b.high.band2));
//     }
//     res = eq_flt(res,  &(st->eq2), &(coef->eq2));
//     res = compressor(res, &(st->limiter), &(coef->limiter));
    
//     // band4_t b = cross4b_flt(res, &(st->cross), coef->cross);
//     //     if(coef->comp_1b.Enable){
//     //         b.low.band1 = compressor(b.low.band1, &(st->comp_1b), &(coef->comp_1b));
//     //     }
//     //     if(coef->comp_2b.Enable){
//     //         b.low.band2 = compressor(b.low.band2, &(st->comp_2b), &(coef->comp_2b));
//     //     }
//     //     if(coef->comp_3b.Enable){
//     //          b.high.band1 = compressor(b.high.band1, &(st->comp_3b), &(coef->comp_3b));
//     //     }
//     // b.low.band1 = compressor(b.low.band1, &(st->comp_1b), &(coef->comp_1b));

   
    
   

//     return res;
// }


int32_t chain_fxd_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count){
            // printf("chain_flt_process\n");
    int n = check(coeffs, states, audio, samples_count);
    if(n == -1){
        return -1;
    }

    chanes_t *_audio = (chanes_t *)(audio);
    states_t* st = (states_t*)states;
    chain_flt_coef_t *coef = (chain_flt_coef_t *)coeffs;
    // printf("chain_flt_process\n");
    band4_t R;

    for(uint32_t a_index = 0; a_index < samples_count; a_index++){
        // _audio[a_index].Left = chain_flt(_audio[a_index].Left, &st->Left, coef);
        // _audio[a_index].Left = compressor(_audio[a_index].Left, &st->Left, &coef->limiter);
        R = cross4b_fxd(_audio[a_index].Left, &st->Left.cross, coef->cross);
        _audio[a_index].Left = R.high.band1 + R.low.band1;
        _audio[a_index].Right = R.high.band2 + R.low.band2;
    //    _audio[a_index].Left = apf_flt_2nd(_audio[a_index].Left, &st->Left.cross.apf_2nd[0],  &coef->cross.apf_2nd[0]);
        // _audio[a_index].Left = R.band1 + R.band3;// - R.band3 - R.band4;
        // _audio[a_index].Right = R.band2 + R.band4;
        // _audio[a_index].Left = eq_flt(_audio[a_index].Left,  &(st->Left.eq), &(coef->eq));
        // // _audio[a_index].Right = (_audio[a_index].Right +  _audio[a_index].Left)*0.5f;
        // _audio[a_index].Right = (audio_type)coef->apf_dbl((double)_audio[a_index].Left, &(_st->Right), &(coef->apf_coef));
        
    }

    return 0;
}
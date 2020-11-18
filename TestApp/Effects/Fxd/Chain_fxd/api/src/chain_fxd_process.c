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
       eq_fxd_set_state(&st->Left.eq1);
    cross_fxd_set_state(&st->Left.cross);

    // comp_fxd_set_state(&st->Left.comp_1b);
    // comp_fxd_set_state(&st->Left.comp_1b);
    // comp_fxd_set_state(&st->Left.comp_2b);
    // comp_fxd_set_state(&st->Left.comp_3b);
    // comp_fxd_set_state(&st->Left.comp_4b);
      eq_fxd_set_state(&st->Left.eq2);
    comp_fxd_set_state(&st->Left.limiter);

       eq_fxd_set_state(&st->Right.eq1);
    cross_fxd_set_state(&st->Right.cross);

    // comp_fxd_set_state(&st->Right.comp_1b);
    // comp_fxd_set_state(&st->Right.comp_1b);
    // comp_fxd_set_state(&st->Right.comp_2b);
    // comp_fxd_set_state(&st->Right.comp_3b);
    // comp_fxd_set_state(&st->Right.comp_4b);
      eq_fxd_set_state(&st->Right.eq2);
    comp_fxd_set_state(&st->Right.limiter);
    printf("chain_fxd_reset\n");
    
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

// static audio_type apf_fxd_2nd(audio_type x, apf_states_2nd_t *st, apf_coef_2nd_t *coef){
//     audio_type  y = fxd_mac(st->xh[0], coef->k2, x);
    
//     st->xh[0] = fxd_mac(st->xh[1], coef->k1, x);
//     st->xh[0] = fxd_msub(st->xh[0], coef->k1, y);

//     st->xh[1] = fxd_msub(x, coef->k2, y);

//     return y;
// }

static inline audio_type chain_fxd(audio_type x, chain_fxd_states_t *st, chain_fxd_coef_t *coef){
    audio_type res = eq_fxd(x,  &(st->eq1), &(coef->eq1));

    if (coef->comp_4b_enable)
    {
        band4_t b = cross4b_fxd(res, &(st->cross), coef->cross);
        if(coef->comp_1b.Enable){
            b.low.band1 = compressor_fxd(b.low.band1, &(st->comp_1b), &(coef->comp_1b));
        }
        if(coef->comp_2b.Enable){
            b.low.band2 = compressor_fxd(b.low.band2, &(st->comp_2b), &(coef->comp_2b));
        }
        if(coef->comp_3b.Enable){
             b.high.band1 = compressor_fxd(b.high.band1, &(st->comp_3b), &(coef->comp_3b));
        }
        if(coef->comp_3b.Enable){
            b.high.band2 = compressor_fxd(b.high.band2, &(st->comp_4b), &(coef->comp_4b));
        }
        res = fxd_add( fxd_add(b.low.band1,0/* b.low.band2*/), fxd_add(b.high.band1,0 /*b.high.band2*/));
    }

    res = eq_fxd(res,  &(st->eq2), &(coef->eq2));
    res = compressor_fxd(res, &(st->limiter), &(coef->limiter));
    
//     // band4_t b = cross4b_fxd(res, &(st->cross), coef->cross);
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

   
    
   

    return res;
}


int32_t chain_fxd_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count){
            // printf("chain_fxd_process\n");
    int n = check(coeffs, states, audio, samples_count);
    if(n == -1){
        return -1;
    }

    chanes_t *_audio = (chanes_t *)(audio);
    states_t* st = (states_t*)states;
    chain_fxd_coef_t *coef = (chain_fxd_coef_t *)coeffs;
    // printf("chain_fxd_process\n");
    // band4_t R;

    for(uint32_t a_index = 0; a_index < samples_count; a_index++){
        _audio[a_index].Left = chain_fxd(_audio[a_index].Left, &st->Left, coef);
        // _audio[a_index].Left = compressor(_audio[a_index].Left, &st->Left, &coef->limiter);
        // R = cross4b_fxd(_audio[a_index].Left, &st->Left.cross, coef->cross);
        // _audio[a_index].Left = R.low.band1 ;
        // _audio[a_index].Right = R.low.band2;
    //    _audio[a_index].Left = apf_fxd_2nd(_audio[a_index].Left, &st->Left.cross.apf_2nd[0],  &coef->cross.apf_2nd[0]);
        // _audio[a_index].Left = R.band1 + R.band3;// - R.band3 - R.band4;
        // _audio[a_index].Right = R.band2 + R.band4;
        // _audio[a_index].Left = eq_fxd(_audio[a_index].Left,  &(st->Left.eq1), &(coef->eq1));
        // _audio[a_index].Left = compressor_fxd(_audio[a_index].Left, &(st->Left.limiter), &(coef->limiter));
        // // _audio[a_index].Right = (_audio[a_index].Right +  _audio[a_index].Left)*0.5f;
        // _audio[a_index].Right = (audio_type)coef->apf_dbl((double)_audio[a_index].Left, &(_st->Right), &(coef->apf_coef));
        
    }

    return 0;
}

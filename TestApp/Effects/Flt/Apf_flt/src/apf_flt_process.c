#include "apf_flt.h"
#include <stdlib.h>
#include <Windows.h>

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


audio_type apf_direct_form2(audio_type x, apf_states_t *st, apf_coefs_t *coef){
    // audio_type y = 0;
    audio_type y[APF_2nd + 1];
    audio_type Xh[APF_2nd + 1];
    Xh[APF_2nd] = x;
    y[APF_2nd] = st->xh[APF_2nd - 1];

    for(int order = (APF_2nd - 1); order >= 0 ; order--){
        Xh[order] = flt_msub(Xh[order + 1], st->xh[order], coef->c[order]);
        y[order] = flt_mac(st->xh[order], Xh[order], coef->c[order]);
    }
    for(int order = (APF_2nd - 1); order >= 0 ; order--){
        st->xh[order] = Xh[order];
    }
    return y[0];
}

audio_type apf_direct_form(audio_type x, apf_states_t *st, apf_coefs_t *coef){
    // audio_type y = 0;
    audio_type y[APF_ORDER + 1];
    audio_type Xh[APF_ORDER + 1];
    Xh[APF_ORDER] = x;
    y[APF_ORDER] = st->xh[APF_ORDER - 1];

    for(int order = (APF_ORDER - 1); order >= 0 ; order--){
        Xh[order] = flt_mac(Xh[order + 1], st->xh[order], flt_neg(coef->c[order]));
        y[order] = flt_mac(st->xh[order], Xh[order], coef->c[order]);
    }
    for(int order = (APF_ORDER - 1); order >= 0 ; order--){
        st->xh[order] = Xh[order];
    }
    return y[0];
}

// audio_type apl_direct_recurs(audio_type x, apf_states_t *st, apf_coefs_t *coef, int order){
//     audio_type y = 0;
//     st->xh[order] =
//     return ;
// }

// audio_type apl_2nd_order(audio_type x, xh_z_t *xh, apf_coef_2nd_t *coef){
//     audio_type  xh_z0 = x;
//     xh_z0 = flt_msub(xh_z0, coef->dc, xh->z_1);
//     xh_z0 = flt_mac(xh_z0, coef->c, xh->z_2);
//     // Sleep(100);
//     // printf("xh_z0 %f\n",  xh_z0);

//     audio_type  y = flt_mul(coef->dc, xh->z_1);
//     y = flt_msub(y, coef->c, xh_z0);
//     y = flt_add(y, xh->z_2);    

//     xh->z_2 = xh->z_1;
//     xh->z_1 = xh_z0;
//     return y;
// }

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
    apf_coefs_t* coef = (apf_coefs_t*)coeffs;

    for(uint32_t a_index = 0; a_index < samples_count; a_index++){
        _audio[a_index].Left = apf_direct_form(_audio[a_index].Left, &(_st->Left), coef);
        _audio[a_index].Right =  (_audio[a_index].Left -_audio[a_index].Right )*0.5f;//  apf_direct_form2(_audio[a_index].Right, &(_st->Right), coef);
    }

    return 0;
}

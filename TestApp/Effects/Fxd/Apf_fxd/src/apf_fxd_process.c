#include "apf_fxd.h"
#include <stdlib.h>
#include <Windows.h>

/******************************************************************************/

int32_t apf_fxd_process_get_sizes(
    size_t*     states_bytes){

    *states_bytes = sizeof(states_t);

    return 0;
}

/*******************************************************************************/

int32_t apf_fxd_reset(
    void const* coeffs,
    void*       states){

    states_t *_st = (states_t *)states;
    for(int order = (APF_ORDER - 1); order >= 0 ; order--){
        _st->Left.xh[order] = 0;
        _st->Right.xh[order] = 0;
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

static audio_type apl_1st_order(audio_type x, apf_states_t *st, apf_coefs_t *coef){
    //  x = INT32_MIN;
    acum_type xh = fxd_fmul(coef->c[0], st->xh[0]);
    xh = fxd63_rshift(xh, COEF_FR); //convert to type type 'xh'
    x = fxd_rshift(x, 5); //convert to type type xh
    xh = fxd63_sub(x, xh);
    saturation(xh);

    // exit(1);
    // xh = fxd63_rshift(xh, 3); //convert to type type Q29

    acum_type y = fxd_fmul(coef->c[0], (audio_type)xh);
    y = fxd63_rshift(y, COEF_FR); //convert to type type 'xh'
    y = fxd63_add(y, st->xh[0]);
    y = fxd63_lshift(y, 10); //convert to type type Q31
    // y = saturation(y);


    st->xh[0] = (audio_type)xh;

    return  (audio_type)y;
}

static audio_type apl_direct_2nd_order(audio_type x, apf_states_t *st, apf_coefs_t *coef){
    x = INT32_MIN + 5 ;
    acum_type xh = fxd63_lshift(x, (COEF_FR - 16)); //convert to type type 'xh'
    // print_n63(xh);
    xh = fxd_msub_m(xh, coef->c[1], st->xh[0]);
    xh = fxd_mac_m(xh, coef->c[0], st->xh[1]);
    // print_n63(xh);
    xh = fxd63_rshift(xh, COEF_FR);
    xh = saturation(xh);
    // print_n63(xh);

    acum_type y = fxd_fmul(coef->c[1], st->xh[0]);
    // y = fxd_msub_m(y, coef->c[0], (audio_type)xh);
    // y = fxd63_rshift(y, COEF_FR);                   //convert to type type 'xh'
    // y = fxd63_add((audio_type)y, st->xh[1]);
    // y = fxd63_lshift(y, 12);
    // y = saturation(y);


    st->xh[1] = st->xh[0];
    st->xh[0] = (audio_type)xh;
    // printf("st->xh[0] %d\n", st->xh[0]);

// exit(0);
    return (audio_type)y;
}

static audio_type apl_lattice_2nd_order(audio_type x, apf_states_t *st, apf_coefs_t *coef){
    acum_type xh = fxd_fmul(coef->c[1], st->xh[1]);
    xh = fxd63_rshift(xh, COEF_FR); //convert xh type
    x = fxd_rshift(x, 10);   //convert xh type
    xh =  fxd63_sub(x, xh);
    xh = saturation(xh);

    acum_type  y = fxd_fmul(coef->c[1], (audio_type)xh);
    y = fxd63_rshift(y, COEF_FR); //convert xh type
    y = fxd63_add(y, st->xh[1]);
    y = fxd63_lshift(y, 10); //convert to type type Q31
    y = saturation(y);

    st->xh[1] = apl_1st_order((audio_type)xh, st, coef); //type Q31
    // Sleep(100);
    // printf("st->xh[1] %d\n", st->xh[1]);

    return (audio_type)y;
}

// static audio_type apl_lattice_2nd_order_dbl(audio_type x, apf_states_t *st, apf_coefs_t *coef){
//     double xh = x;

//     xh -= coef->c_dbl[1] *  st->xh_dbl[1];
//     double y = st->xh_dbl[1] + (coef->c_dbl[1] * xh);

//     st->xh_dbl[1] = apl_1st_dbl(xh, st, coef);

//     return (audio_type)y;
// }

int32_t apf_fxd_process(
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

    switch (coef->order)
    {
    case 1:
        for(uint32_t a_index = 0; a_index < samples_count; a_index++){
            _audio[a_index].Left = apl_1st_order(_audio[a_index].Left, &(_st->Left), coef);
            // _audio[a_index].Right = (_audio[a_index].Left -_audio[a_index].Right);
        }
        break;
    case 2:
        switch(coef->form)
        {
        case 1:
            for(uint32_t a_index = 0; a_index < samples_count; a_index++){
                _audio[a_index].Left = apl_direct_2nd_order(_audio[a_index].Left, &(_st->Left), coef);
                // _audio[a_index].Right = ((_audio[a_index].Left >> 1) + (_audio[a_index].Right>>1));//  apf_direct_form2(_audio[a_index].Right, &(_st->Right), coef);
            }
            break;
        case 2:
            for(uint32_t a_index = 0; a_index < samples_count; a_index++){
                _audio[a_index].Left = apl_lattice_2nd_order(_audio[a_index].Left, &(_st->Left), coef);
                _audio[a_index].Right = ((_audio[a_index].Left >> 1) + (_audio[a_index].Right>>1));//  apf_direct_form2(_audio[a_index].Right, &(_st->Right), coef);
            }
            break;
        default:
            break;
        }
    default:
        break;
    }

    

    return 0;
}

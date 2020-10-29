#include "cross_fxd.h"
#include <stdlib.h>
#include <Windows.h>

#define SUM_BITS        (COEF_FR - 14)
#define FORWARD_MASK    ((1u << COEF_FR) - 1)
#define BACK_MASK       ((1u << SUM_BITS) - 1)

/******************************************************************************/

int32_t cross_fxd_process_get_sizes(
    size_t*     states_bytes){

    *states_bytes = sizeof(states_t);

    return 0;
}

/*******************************************************************************/

int32_t cross_fxd_reset(
    void const* coeffs,
    void*       states){

    states_t *_st = states;
    for(int i = 0; i < (NUM_OF_FREQ * 2); i++){
        for(int o = 0; o < 2; o++){
            _st->Left.apf_1st[i].xh[o] = 0;
            _st->Left.apf_2nd[i].xh[o] = 0;
            _st->Right.apf_1st[i].xh[o] = 0;
            _st->Right.apf_2nd[i].xh[o] = 0;
        }
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


static audio_type apl_1st_order(audio_type x, apf_states_t *st, apf_coef_t *coef){
    acum_type xh = fxd63_lshift(x, SUM_BITS); //convert to type type xh
    xh = fxd63_add(xh,st->noise[0]);
        // printf("c = %d\t", coef->k[0]);
        // exit(0);
    xh =  fxd_msub_m(xh, coef->k[0], st->xh[0]);

    st->noise[0] = (fxd_q31_t)(xh & FORWARD_MASK);
    xh = fxd63_rshift(xh, (COEF_FR)); //convert to type type Q30

    // saturation(xh);

    acum_type y = fxd63_lshift(st->xh[0], COEF_FR);
    y = fxd_mac_m(y, coef->k[0], (audio_type)xh);
    y = fxd63_add(y,st->noise[1]);
    st->noise[1] = (fxd_q31_t)(y & BACK_MASK);
    y = fxd63_rshift(y, SUM_BITS);    //convert to type type Q31

    y = saturation(y);

    st->xh[0] = (audio_type)xh;

    return  (audio_type)y;
}

static audio_type apl_2nd_order(audio_type x, apf_states_t *st, apf_coef_t *coef){
    acum_type xh = fxd63_lshift(x, SUM_BITS);
    xh = fxd63_add(xh,st->noise[2]);
    xh = fxd_msub_m(xh, coef->k[1], st->xh[1]);

    st->noise[2] = (fxd_q31_t)(xh & FORWARD_MASK);
    
    xh = fxd63_rshift(xh, COEF_FR);         //convert xh type
    saturation(xh);

    acum_type  y = fxd63_lshift(st->xh[1], COEF_FR);
    y = fxd63_add(y, st->noise[3]);
    y = fxd_mac_m(y, coef->k[1], (audio_type)xh);
    st->noise[3] = (fxd_q31_t)(y & BACK_MASK);
    y = fxd63_rshift(y, SUM_BITS);
    
    y = saturation(y);

    st->xh[1] = apl_1st_order((audio_type)xh, st, coef); //type Q31

    return (audio_type)y;
}

static audio_type calc_crossover(audio_type x, cross_states_t *st, coef_t *coef){
    audio_type band1 = 0;
    audio_type band2 = 0;
    audio_type band3 = 0;
    audio_type band4 = 0;

    audio_type  high = 0;
    audio_type  xh = fxd_rshift(x,  1);
    audio_type  apl_2nd = apl_2nd_order(xh,       &(st->apf_2nd[0]), &coef->apf_2nd[1]);
    audio_type  apl_1st = apl_1st_order(xh,       &(st->apf_1st[0]), &coef->apf_1st[1]);

    audio_type  low = fxd_add(apl_2nd, apl_1st);
    low = fxd_rshift(low,  1);
                apl_2nd = apl_2nd_order(low, &(st->apf_2nd[3]), &coef->apf_2nd[1]);
                apl_1st = apl_1st_order(low, &(st->apf_1st[3]), &coef->apf_1st[1]);

    low = fxd_add(apl_2nd, apl_1st);
    
    high = fxd_sub(x, low);

    xh = fxd_rshift(low, 1);

    apl_2nd = apl_2nd_order(xh, &(st->apf_2nd[1]), &coef->apf_2nd[0]);   
    apl_1st = apl_1st_order(xh, &(st->apf_1st[1]), &coef->apf_1st[0]);

    band1 = fxd_add(apl_2nd, apl_1st);
    band1 = fxd_rshift(band1,  1);
    
        apl_2nd = apl_2nd_order(band1, &(st->apf_2nd[4]), &coef->apf_2nd[1]);
        apl_1st = apl_1st_order(band1, &(st->apf_1st[4]), &coef->apf_1st[1]);

    band1 = fxd_add(apl_2nd, apl_1st);

    band2 = fxd_sub(low, band1);

    xh = fxd_rshift(high, 1);

    apl_2nd = apl_2nd_order(xh, &(st->apf_2nd[2]), &coef->apf_2nd[2]);   
    apl_1st = apl_1st_order(xh, &(st->apf_1st[2]), &coef->apf_1st[2]);

    band3 = fxd_add(apl_2nd, apl_1st);
    band3 = fxd_rshift(band3, 1);

        apl_2nd = apl_2nd_order(band3, &(st->apf_2nd[5]), &coef->apf_2nd[1]);
        apl_1st = apl_1st_order(band3, &(st->apf_1st[5]), &coef->apf_1st[1]);

    band3 = fxd_add(apl_2nd, apl_1st);
    band4 = high - band3;

    acum_type res = 0;
    res = fxd_mac(res, band1, coef->gain_c[0]);
    res = fxd_mac(res, band2, coef->gain_c[1]);
    res = fxd_mac(res, band3, coef->gain_c[2]);
    res = fxd_mac(res, band4, coef->gain_c[3]);

    return band1+band2 + band3 + band4;//fxd_get_high(res);
}

int32_t cross_fxd_process(
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
    coef_t* coef = (coef_t*)coeffs;

    for(uint32_t a_index = 0; a_index < samples_count; a_index++){
        _audio[a_index].Left = calc_crossover(_audio[a_index].Left, &(_st->Left), coef);
        //_audio[a_index].Right = ;
    }

    return 0;
}

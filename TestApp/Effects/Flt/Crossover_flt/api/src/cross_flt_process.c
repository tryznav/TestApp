#include "cross_api.h"
#include <stdlib.h>
#ifdef WIN32
    #include <Windows.h>
#else
    #include <unistd.h>  
#endif

/******************************************************************************/
int32_t cross_flt_process_get_sizes(
    size_t*     states_bytes){

    *states_bytes = sizeof(states_t);

    return 0;
}
/*******************************************************************************/

int32_t cross_flt_reset(
    void const* coeffs,
    void*       states){

    states_t *_st = states;
    for(int i = 0; i < (NUM_OF_FREQ * 2); i++){
        for(int o = 0; o < 2; o++){
            _st->Left.apf_1st[i].xh[o] = 0.0f;
            _st->Left.apf_2nd[i].xh[o] = 0.0f;
            _st->Right.apf_1st[i].xh[o] = 0.0f;
            _st->Right.apf_2nd[i].xh[o] = 0.0f;
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

static audio_type cross_hand_flt(bands_t a, coef_type *gain_c){
    audio_type res = 0;
    res = flt_mac(res, a.band1, gain_c[0]);
    res = flt_mac(res, a.band2, gain_c[1]);
    res = flt_mac(res, a.band3, gain_c[2]);
    res = flt_mac(res, a.band4, gain_c[3]);
    return res;
}

static audio_type cross_hand_dbl(bands_dbl_t a, coef_type *gain_c){
    double  res = 0;
    res += a.band1 * gain_c[0];
    res += a.band2 * gain_c[1];
    res += a.band3 * gain_c[2];
    res += a.band4 * gain_c[3];
    return (audio_type)res;
}

int32_t cross_flt_process(
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
        _audio[a_index].Left = cross_hand_flt(crossover_flt(_audio[a_index].Left, &(_st->Left), &(coef->cross_coef)), coef->gain_c);
        // _audio[a_index].Right = cross_hand_dbl(crossover_dbl((double)_audio[a_index].Right, &(_st->Right), &(coef->cross_coef)), coef->gain_c);
    }

    return 0;
}

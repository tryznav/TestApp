#include "effect_process.h"
#include "colors.h"

#include <stdio.h>
#include <math.h>

typedef struct chanes_s{
    float Left;
    float Right;
}chanes_t;

/******************************************************************************/

int32_t effect_process_get_sizes(
    size_t*     states_bytes){

    *states_bytes = sizeof(float);

    return 0;
}

/*******************************************************************************/

int32_t effect_reset(
    void const* coeffs,
    void*       states){

    return 0;

}

/******************************************************************************/

int32_t effect_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count){

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

    chanes_t *audio_ch = (chanes_t *)(audio);

    for(size_t i = 0; i < samples_count; i++){
        audio_ch[i].Left *= *((float *)coeffs);
        audio_ch[i].Right *= *((float *)coeffs);
    }
    return 0;
}

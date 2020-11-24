#include "test_sig_gen.h"

int32_t check_gen(uint32_t length_sample, void* states, void *audio){
    if(!states){
        fprintf(stderr,RED" Error: "BOLDWHITE"NULL pointer 'states'.Rejected.\n"RESET);
        return -1;
    }
    if(!length_sample){
        fprintf(stderr,RED" Error: "BOLDWHITE" 0 'length_sample'.Rejected.\n"RESET);
        return -1;
    }
    if(!audio){
        fprintf(stderr,RED" Error: "BOLDWHITE"NULL pointer 'audio'.Rejected.\n"RESET);
        return -1;
    }
    return 0;
}

static int32_t convert(float n){
    if(n < -1.0){
        return INT32_MIN;
    }
    if(n >= 1.0){
        return INT32_MAX;
    }
    return (int32_t)(n * (1u<<31));
}

int32_t IEEE_754_to_PCM(void *audio, uint32_t length_sample){
    for(uint32_t i = 0; i < (length_sample * 2); i++){
        ((int32_t *)audio)[i] = convert(((float *)audio)[i]);
    }
    return 0;
}


#include "test_sig_gen.h"
#include<time.h>
#include <stdio.h> 
#include <stdlib.h> 

void *tsig_wnoise_init_states (uint32_t sample_rate, uint32_t length_sample, void const *params){
    void    *states = NULL;

    states = malloc(15);
    if(states == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return states;

}


int32_t tsig_gen_wnoise_st(uint32_t sample_rate, uint32_t length_sample, float amplitude_coef, void const *params, void* states, void *audio){
    // printf("im here \n");
    // srand((unsigned int)time(0));
    for (uint32_t i = 0; i < length_sample ; i++){
        //  ((chanels_t *)audio)[i].Left = (float)rand();
        ((chanels_t *)audio)[i].Left =((float)rand()/(float)(RAND_MAX)*2.0f - 1.0f);
        // printf("(chanels_t *)audio)[%d].Left = %f  %f\n", i,((chanels_t *)audio)[i].Left, amplitude_coef);
        ((chanels_t *)audio)[i].Left *= amplitude_coef;
        // //printf(" %f\n",((chanels_t *)audio)[i].Left);
        ((chanels_t *)audio)[i].Right = ((chanels_t *)audio)[i].Left;//((chanels_t *)audio)[i].Left;
        // ((float *)audio)[i] = ((float)rand()/(float)(RAND_MAX)*2.0f - 1.0f) * amplitude_coef;
        // printf("((float *)audio)[%d] = %f\n",i, ((float *)audio)[i]);
    }
        printf("im here \n");

    return 0;
}

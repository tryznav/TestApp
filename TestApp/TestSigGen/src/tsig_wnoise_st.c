
#include "test_sig_gen.h"

float *tsig_wnoise_st(uint32_t sample_rate, uint32_t length_sample, void *params, void* states, void *audio){
    sample_rate = 0;
    float coef = powf(10.0f , (0.05f * ((tsig_wnoise_prm_t *)params)->amplitude));

    for (uint32_t i = 0; i < length_sample; i++){
        ((chanels_t *)audio)[i].Right = ((float)rand()/(float)(RAND_MAX) * coef);
        ((chanels_t *)audio)[i].Left = 0.0f;
    }

}

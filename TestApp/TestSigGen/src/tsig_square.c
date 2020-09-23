#include "test_sig_gen.h"

void *tsig_square_init_states (uint32_t sample_rate, uint32_t length_sample, void const *params){
    tsig_square_stat_t *states = NULL;
    tsig_square_prm_t * _params = (tsig_square_prm_t *) params;
    if(_params == NULL){
        fprintf(stderr,RED" Error: "BOLDWHITE"NULL pointer params.Rejected.\n"RESET);
        return NULL;
    }

    states = malloc(sizeof(tsig_square_stat_t));
    if(states == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    states->sample_increment = 0;
    states->sign_value = 1;
    states->half_period_sample = (sample_rate/2000) * (((tsig_square_prm_t *)params)->period_ms);
    printf("sample_rate = %d\n", sample_rate);
    return states;
}

int32_t tsig_gen_square_st(uint32_t sample_rate, uint32_t length_sample, float amplitude_coef, void const *params, void* states, void *audio){
    tsig_square_stat_t * _states = (tsig_square_stat_t *)states;
    
    if(!_states){
        fprintf(stderr,RED" Error: "BOLDWHITE"NULL pointer _states.Rejected.\n"RESET);
        return -1;
    }
    for (uint32_t i = 0; i < length_sample; i++){
        ((chanels_t *)audio)[i].Left = ((tsig_square_stat_t *)(states))->sign_value * amplitude_coef;
        ((chanels_t *)audio)[i].Right = ((chanels_t *)audio)[i].Left;

        _states->sample_increment++;
        if(_states->sample_increment == _states->half_period_sample){
            _states->sample_increment = 0;
            _states->sign_value *= -1;
        }
    }

    return 0;
}

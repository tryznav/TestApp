#ifndef __CROSSOVERS_H__
#define __CROSSOVERS_H__

#include "frame_types.h"
#include "flt_arithmetic.h"
#include "filters.h"

typedef struct crossover2b_params_s{
    double      freq;
    double      sample_rate;
}crossover2b_params_t;

typedef struct crossover2b_coeffs_s{
    vfloat4_t               k;//1st order      
    vfloat4_t               k1;//2nd order
    vfloat4_t               k2;//2nd order
    vfloat4_t               half;
}crossover2b_coeffs_t;

typedef struct crossover2b_states_s{
    vfloat4_t               xh;//1st order -3db
    vfloat4_t               xh1;//1st order -3db
    vfloat4_t               xh2;//1st compensation

    vfloat4_t               bands2[FRAME_SIZE];
}crossover2b_states_t;

typedef struct crossover2b_to_4b_params_s{
    double      freq[2];
    double      sample_rate;
}crossover2b_to_4b_params_t;

typedef struct crossover2b_to_4b_coeffs_s{
    vfloat4_t               k;//1st order      
    vfloat4_t               k1;//2nd order
    vfloat4_t               k2;//2nd order
    vfloat4_t               half;
}crossover2b_to_4b_coeffs_t;

typedef struct crossover2b_to_4b_states_s{
    vfloat4_t               xh;//1st order -3db
    vfloat4_t               xh1;//1st order -3db
    vfloat4_t               xh2;//1st compensation

    vfloat8_t               bands4[FRAME_SIZE];
}crossover2b_to_4b_states_t;

/***************************************************/

typedef struct crossover4b_params_s{
    crossover2b_to_4b_params_t  crossover2b_to_4b;
    crossover2b_params_t        crossover2b;
    apf2_band2_params_t         apf_compens;
}crossover4b_params_t;

typedef struct crossover4b_coeffs_s{
    crossover2b_to_4b_coeffs_t  crossover2b_to_4b;
    crossover2b_coeffs_t        crossover2b;
    apf2_band2_coeffs_t         apf_compens;
}crossover4b_coeffs_t;

typedef struct crossover4b_states_s{
    crossover2b_to_4b_states_t  crossover2b_to_4b;
    crossover2b_states_t        crossover2b;
    apf2_band2_states_t         apf_compens;
    vfloat8_t                   band4;
}crossover4b_states_t;


#endif
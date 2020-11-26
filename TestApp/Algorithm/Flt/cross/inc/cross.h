#ifndef __CROSS_H__
#define __CROSS_H__

#include "colors.h"
#include "flt_arithmetic.h"
// #include "params_id.h"
#include "algorithm.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif



#define ORDER           2
#define NUM_OF_FREQ     3
#define NUM_OF_BAND     (NUM_OF_FREQ + 1)

typedef struct cross_prm_s{
    double          freq[NUM_OF_FREQ];
    double          sample_rate;
}cross_prm_t;



#if AVX
    typedef vfloat band2_t;
    typedef vfloat band4_t;
#elif SSE
    typedef struct band4_s{
        vfloat;
    }band2_t;
    typedef struct band4_s{
        band2_t low;
        band2_t high;
    }band4_t;
#else
    #if CH == 2
        typedef struct band2_s{
            vfloat  band1; //low
            vfloat  band2; //high
        }band2_t;

        typedef struct band4_s{
        band2_t low;
        band2_t high;
        }band4_t;
    #elif CH == 4
        typedef struct band2_s{
            vfloat  band1; //low
            vfloat  band2; //high
        }band2_t;

        typedef struct band4_s{
        band2_t low;
        band2_t high;
        }band4_t;
    #elif CH == 8
        typedef vfloat band2_t;
        typedef vfloat band4_t;
    #endif
#endif






#if CH == 2

typedef struct cross2b_coef_s{
    vfloat    k;//1st order
    vfloat    k1;//2nd order
    vfloat    k2;//2nd order
    vfloat    half;
}cross2b_coef_t;

typedef struct co_states_s{
    vfloat xh1;//1st order -3db
    vfloat xh2;//1st compensation
}co_states_t;
typedef struct cross4b_coef_s{
    cross2b_coef_t    f[3];
}cross4b_coef_t;


typedef struct cross2b_states_s{
    vfloat xh;//1st order -3db
    vfloat xh1;//1st order -3db
    vfloat xh2;//1st compensation
}cross2b_states_t;

typedef struct cross4b_states_s{
    cross2b_states_t    f0;
    cross2b_states_t    f1;
    cross2b_states_t    f2;
    co_states_t         c_f0;
    co_states_t         c_f2;
    co_states_t         c_f22;
}cross4b_states_t;
#endif

#if CH == 8

typedef struct cross2b_coef_s{
    vfloat    k;//1st order
    vfloat    k1;//2nd order
    vfloat    k2;//2nd order
    vfloat    half;
}cross2b_coef_t;

typedef struct cross2b_to_4b_coef_t{
    vfloat    k1_c;
    vfloat    k2_c;
    vfloat    k;//1st order
    vfloat    k1;//2nd order
    vfloat    k2;//2nd order
    vfloat    half;
}cross2b_to_4b_coef_t;

typedef struct co_states_s{
    vfloat xh1;//1st order -3db
    vfloat xh2;//1st compensation
}co_states_t;
typedef struct cross4b_coef_s{
    cross2b_coef_t          f1;
    cross2b_to_4b_coef_t    f0_f2;
}cross4b_coef_t;

typedef struct cross2b_states_s{
    vfloat xh;//1st order -3db
    vfloat xh1;//1st order -3db
    vfloat xh2;//1st compensation
}cross2b_states_t;

typedef struct cross2b_to_4b_states_s{
    vfloat xh;//1st order -3db
    vfloat xh1;//1st order -3db
    vfloat xh2;//1st compensation
    vfloat xh1_c;//compensation
    vfloat xh2_c;//compenstaion
}cross2b_to_4b_states_t;

typedef struct cross4b_states_s{
    cross2b_states_t         f1;
    cross2b_to_4b_states_t   f0_f2;
}cross4b_states_t;
#endif




void cross_calculate_coef(cross_prm_t *prm, cross4b_coef_t *coef);
void cross_set_state(cross4b_states_t *st);
void cross_set_prm_d(cross_prm_t *prm, double samle_rate);
int32_t cross_set_prm_c(cross_prm_t *prm, id_union_t id_prm, float value);
void cross4b_process(vfloat *x, band4_t *res, cross4b_coef_t *coef, cross4b_states_t *st);


#endif
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



#if CH == 2
    static const vfloat HALF_A = {0.5f, 0.5f};
    static const vfloat TWO_A = {2.0f, 2.0f};
#endif

#if CH == 4
    const vfloat HALF_A = {0.5f, 0.5f, 0.5f, 0.5f};
#endif

typedef struct cross_prm_s{
    double          freq[NUM_OF_FREQ];
    double          sample_rate;
}cross_prm_t;



#if AVX
    typedef struct band4_s{
        vfloat bands;
    }band2_t;
    typedef struct band4_s{
        vfloat bands;
    }band4_t;
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
    #endif
#endif





typedef struct cross2b_coef_s{
    vfloat    k;//1st order
    vfloat    k1;//2nd order
    vfloat    k2;//2nd order
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


void cross_calculate_coef(cross_prm_t *prm, cross4b_coef_t *coef);
void cross_set_state(cross4b_states_t *st);
void cross_set_prm_d(cross_prm_t *prm, double samle_rate);
int32_t cross_set_prm_c(cross_prm_t *prm, id_union_t id_prm, float value);
void cross4b_process(vfloat *x, band4_t *res, cross4b_coef_t *coef, cross4b_states_t *st);


#endif
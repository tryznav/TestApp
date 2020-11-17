#ifndef __CROSS_FLT_H__
#define __CROSS_FLT_H__

#include "colors.h"
#include "flt_arithmetic.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif



#define ORDER           2
#define NUM_OF_FREQ     3
#define NUM_OF_BAND     (NUM_OF_FREQ + 1)

typedef my_float audio_type;
typedef my_float coef_type;
typedef my_float acum_type;

#pragma pack(push,1)
typedef struct chanes_s{
    my_float Left;
    my_float Right;
}chanes_t;
#pragma pack(pop)

typedef struct cross_prm_s{
    double          freq[NUM_OF_FREQ];
    double          sample_rate;
}cross_prm_t;

typedef struct band2_s{
    audio_type  band1; //low
    audio_type  band2; //high
}band2_t;

typedef struct band4_s{
    band2_t low;
    band2_t high;
}band4_t;

typedef struct cross_flt_coef_t{
    my_float    k1_1;//1st order
    my_float    k1_2;//2nd order
    my_float    k2_2;//2nd order
}cross_flt_coef_t;


typedef struct co_states_t{
    my_float f;//1st order
    my_float s[2];//2nd order
}co_states_t;

typedef struct cross2b_states_s{
    my_float f_1;//1st order -3db
    my_float f_2;//1st order -3db
    my_float s_1[2];//2nd order -3db
    my_float s_2[2];//2nd order -6db
    my_float f_c;//1st compensation
}cross2b_states_t;

typedef struct cross4b_states_s{
    cross2b_states_t    f0;
    cross2b_states_t    f1;
    cross2b_states_t    f2;
    co_states_t         c_f0;
    co_states_t         c_f2;
}cross4b_states_t;

typedef struct cross_flt_states_s{
    cross4b_states_t Left;
    cross4b_states_t Right;
}cross_flt_states_t;

int32_t cross_flt_coef(cross_prm_t *prm, cross_flt_coef_t *coef);
int32_t cross_flt_set_state(cross4b_states_t *st);
int32_t cross_flt_set_prm_d(cross_prm_t *prm, double samle_rate);
int32_t cross_flt_set_prm_c(cross_prm_t *prm, uint8_t Id_prm, float value);
band4_t cross4b_flt(audio_type x, cross4b_states_t *st, cross_flt_coef_t *coef);


#endif
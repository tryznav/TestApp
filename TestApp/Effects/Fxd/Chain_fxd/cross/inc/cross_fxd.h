#ifndef __CROSS_FXD_H__
#define __CROSS_FXD_H__

#include "colors.h"
#include "fxd_arithmetic.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#define COEF_FR         31

#define ORDER           2
#define NUM_OF_FREQ     3
#define NUM_OF_BAND     (NUM_OF_FREQ + 1)

typedef fxd_q31_t audio_type;
typedef fxd_q31_t coef_type;
typedef fxd_q63_t acum_type;

#pragma pack(push,1)
typedef struct chanes_s{
    fxd_q31_t Left;
    fxd_q31_t Right;
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

typedef struct cross_fxd_coef_s{
    fxd_q31_t    k1_1;//1st order
    fxd_q31_t    k1_2;//2nd order
    fxd_q31_t    k2_2;//2nd order
}cross_fxd_coef_t;

typedef struct co_fxd_states_t{
    fxd_q31_t f[2];//1st order
    fxd_q31_t s[4];//2nd order
}co_fxd_states_t;

typedef struct cross2b_fxd_states_s{
    fxd_q31_t f_1[2];//1st order -3db
    fxd_q31_t f_2[2];//1st order -3db
    fxd_q31_t s_1[4];//2nd order -3db
    fxd_q31_t s_2[4];//2nd order -6db
    fxd_q31_t f_c[2];//1st compensation
}cross2b_fxd_states_t;

typedef struct cross4b_fxd_states_s{
    cross2b_fxd_states_t    f0;
    cross2b_fxd_states_t    f1;
    cross2b_fxd_states_t    f2;
    co_fxd_states_t         c_f0;
    co_fxd_states_t         c_f2;
}cross4b_fxd_states_t;

typedef struct cross_fxd_states_t{
    cross4b_fxd_states_t Left;
    cross4b_fxd_states_t Right;
}cross_fxd_states_t;

int32_t cross_fxd_coef(cross_prm_t *prm, cross_fxd_coef_t *coef);
int32_t cross_fxd_set_state(cross4b_fxd_states_t *st);
int32_t cross_fxd_set_prm_d(cross_prm_t *prm, double samle_rate);
int32_t cross_fxd_set_prm_c(cross_prm_t *prm, uint8_t Id_prm, float value);
band4_t cross4b_fxd(audio_type x, cross4b_fxd_states_t *st, cross_fxd_coef_t *coef);


#endif
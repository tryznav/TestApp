#ifndef __FILTERS_H__
#define __FILTERS_H__

#include "frame_types.h"
#include "flt_arithmetic.h"

#define LPF         0
#define HPF         1
#define BPF         2
#define BSF         3
#define PQE         4
#define LSH         5
#define HSH         6
#define APF         7
#define OFF         8

typedef struct biquad_params_s{
    double          sample_rate;
    double          freq;
    double          gain; 
    double          Q;
    int32_t         type;
    int32_t         Enable;
}biquad_params_t;

typedef struct biquad_states_s{

    // vfloat2_t          x1;
    // vfloat2_t          x2;
    //*********************
    vfloat4_t        x1;
    vfloat4_t        x2;
}biquad_states_t;

typedef struct biquad_coeffs_s{
    // vfloat2_t        b0;
    // vfloat2_t        b1;
    // vfloat2_t        b2;
    // vfloat2_t        a1;
    // vfloat2_t        a2;
    vfloat4_t        b0;
    vfloat4_t        b1;
    vfloat4_t        b2;
    vfloat4_t        a1;
    vfloat4_t        a2;
}biquad_coeffs_t;

typedef struct biquad_cascade2_params_s{
    biquad_params_t  cascade2[2];
}biquad_cascade2_params_t;

typedef struct biquad_cascade2_coeffs_s{
    int32_t         Enable;
    vfloat4_t        b0;
    vfloat4_t        b1;
    vfloat4_t        b2;
    vfloat4_t        a1;
    vfloat4_t        a2;
}biquad_cascade2_coeffs_t;

typedef struct biquad_cascade2_states_s{
    vfloat4_t        x1;
    vfloat4_t        x2;
}biquad_cascade2_states_t;

typedef struct biquad_cascade4_params_s{
    biquad_params_t  cascade4[4];
}biquad_cascade4_params_t;

typedef struct biquad_cascade4_coeffs_s{
    int32_t         Enable;
    vfloat8_t        b0;
    vfloat8_t        b1;
    vfloat8_t        b2;
    vfloat8_t        a1;
    vfloat8_t        a2;
}biquad_cascade4_coeffs_t;

typedef struct biquad_cascade4_states_s{
    vfloat8_t        x1;
    vfloat8_t        x2;
}biquad_cascade4_states_t;

#endif
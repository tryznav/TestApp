#ifndef __IIR_FXD_H__
#define __IIR_FXD_H__

#include "fxd_arithmetic.h"

#include "colors.h"
#include "string.h"

#include <stdio.h>
#include <math.h>

#define TAP_NUM     256
#define M_PI 3.14159265358979323846

#define COEF_FRACTIONAL_BITS     30

typedef struct sweep_s{
    float               start;
    float               end;
}sweep_t;

union feature{
    float               whole_file;
    sweep_t             sweep;
};

typedef struct iir_prm_s{
    union feature       cutoff_freq;
    double  BW; //BW = f2 − f1 = f0/Q  
    double  f0;
    double  Q;
    double  w;
    double  A;
    double  alpha;  //sin(w0)/2Q
    double  sample_rate;
}iir_prm_t;

typedef struct iir_doub_coefs_s{
	double a0;
	double a1;
	double a2;
	double b0;
	double b1;
	double b2;
}iir_doub_coefs_t;

typedef struct iir_coefs_s{
	fxd_q31_t a0;
	fxd_q31_t a1;
	fxd_q31_t a2;
	fxd_q31_t b0;
	fxd_q31_t b1;
	fxd_q31_t b2;
}iir_coefs_t;

#pragma pack(push,1)
typedef struct chanes_s{
    fxd_q31_t Left;
    fxd_q31_t Right;
}chanes_t;
#pragma pack(pop)

typedef struct iir_states_t{
	chanes_t dizer;
	chanes_t input_1;
	chanes_t input_2;
	chanes_t output_1;
	chanes_t output_2;
}iir_states_t;

#endif
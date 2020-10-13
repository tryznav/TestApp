#ifndef __FIR_FLT_H__
#define __FIR_FLT_H__

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "fir_flt_control.h"
#include "fir_flt_process.h"
#include "colors.h"
#include "fxd_arithmetic.h"

#define PRM_GAIN_dB_ID          'Gain'
#define PRM_SAMPLE_RATE_ID      'SMPR'
#define PRM_FREQ_START_ID       'FSTA'
#define PRM_FREQ_END_ID         'FEND'

#define TAP_NUM     256

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

extern const double bpf_coef[256];

typedef struct sweep_s{
    float               start;
    float               end;
}sweep_t;

union feature{
    float               whole_file;
    sweep_t             sweep;
};

typedef struct lpf_prm_s{
    float               sample_rate;
    union feature       cutoff_freq;
}lpf_prm_t;


#pragma pack(push,1)
typedef struct chanes_s{
    my_float Left;
    my_float Right;
}chanes_t;
#pragma pack(pop)
typedef float fir_coeffs_t;

typedef struct fir_states_s{
    uint8_t     start;              //history ofset max value 256 because 256 tap
    uint8_t     ofset;              //history ofset max value 256 because 256 tap
    chanes_t    history[TAP_NUM];   //history ofset max value 256 because 256 tap
}fir_states_t;

#endif

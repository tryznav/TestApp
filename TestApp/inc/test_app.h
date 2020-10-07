#ifndef __TEST_APP_H__
#define __TEST_APP_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "effect_control.h"
#include "effect_process.h"
#include "params_id.h"
#include "file_handler.h"
#include "getopt.h"
#include "test_sig_gen.h"
#include "colors.h"

#define DEFAULT_SAMPLE_RATE     48000
#define HELP_TEXT_OPT               "Options\n"
#define HELP_TEXT_IN                "   --in            -i  <input-file-path>       = Specify file for processing.\n"
#define HELP_TEXT_OUT               "   --out           -o  <output-file-path>      = Specify a file for processing results.\n"
#define HELP_TEXT_P_HDR             "   --print-hdr     -p  -                       = Print header input WAV.\n"
#define HELP_TEXT_GAIN              "   --gain          -i  <amplitude-dB>          = Gain.\n"
#define HELP_TEXT_LPF               "   --lpf           -l  <cutoff-frequency>      = Low-Pass FIR filter.\n"
#define HELP_TEXT_GENER_1           "   --generator     -g\n"
#define HELP_TEXT_GENER_2           "           <sample-rate>,<length_ms><signal-type>:<parametr>   = Generate test signals.\n"
#define HELP_TEXT_GEN_S_T           "           <signal-type>           <parametr>\n"
#define HELP_TEXT_DELTA             "           delta                   -\n"
#define HELP_TEXT_STEP              "           step                    -\n"
#define HELP_TEXT_SQUARE            "           square                  <period-ms>\n"
#define HELP_TEXT_WNOISE            "           wnoise                  <amplitude-dB>\n"
#define HELP_TEXT_SINE              "           sine                    <frequency-Hz,amplitude-dB>\n"
#define HELP_TEXT_CHIRP_LIN         "           chirp_linear            <start-freq-Hz,end-freq-Hz,amplitude-dB>\n"              
#define HELP_TEXT_CHIRP_LOG         "           chirp_logarithmic       <start-freq-Hz,end-freq-Hz,amplitude_dB>\n"
#define HELP_TEXT_lSWEEP            "           lsweep                  <frequency-Hz,start-amp-dB,end-amp-dB>\n"

// "Options
//     --in            -i  <input-file-path>                                   = Specify file for processing.
//     --out           -i  <output-file-path>                                  = Specify a file for processing results.
//     --gain          -i  <amplitude-dB>                                      = Gain.
//     --generator     -g  <sample-rate>,<length_ms><signal-type>:<parametr>   = Generate test signals.
//             <signal-type>           <parametr>
//             selta                   -
//             step                    -
//             square                  <period-ms>
//             wnoise                  <amplitude-dB>
//             sine                    <frequency-Hz,amplitude-dB>
//             chirp_linear            <start-freq-Hz,end-freq-Hz,amplitude-dB>
//             chirp_logarithmic       <start-freq-Hz,end-freq-Hz,amplitude_dB>
//             lsweep                  <frequency-Hz,start-amp-dB,end-amp-dB>"



typedef struct sweep_s{
    float               start;
    float               end;
}sweep_t;

union feature{
    float               whole_file;
    sweep_t             sweep;
};

typedef struct  frequency_t{
    uint32_t            whole_file_freq;
    uint32_t            start_freq;
    uint32_t            end_freq;
}frequency_t;

typedef struct  effect_s{
    union
    {
        float           gain_dB;
        float           cutoff_freq;
    }prm;
}effect_t;

typedef struct  sig_gen_t{
    uint16_t            audioFormatType;
    uint32_t            sample_rate;
    uint32_t            length_ms;
    uint32_t            signal_id;
    union feature       amp_dB;
    union feature       freq_Hz;
    uint32_t            period_ms;
}tgen_t;

typedef struct app_func_s{
    bool            input;
    char            *input_f_path;
    bool            ouput;
    char            *output_f_path;
    tgen_t          *generator;
    effect_t        *effect;
    bool            print_hdr;
}app_func_t;

int app_handler(app_func_t *task);
int32_t app_gain(char *input_f_path, char *output_f_path, float gain_dB);
int32_t app_generator_init(pross_waw_t *pross_waw, app_func_t *task);
int32_t app_gain_init(pross_waw_t *pross_waw,char *input_f_path, char *output_f_path, float gain_dB);
int32_t app_gen_sig_del(pross_waw_t *pr);
int32_t app_gain_del(pross_waw_t *pr);

#endif
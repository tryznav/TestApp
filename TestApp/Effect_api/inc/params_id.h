#ifndef __PARAMS_ID_H__
#define __PARAMS_ID_H__
#include <stdint.h>

#define PRM_GAIN_dB_ID          'Gain'
#define PRM_SAMPLE_RATE_ID      'SMPR'
#define PRM_FREQ_START_ID       'FSTA'
#define PRM_FREQ_END_ID         'FEND'
#define PRM_IIR_TYPE_ID         'TIIR'
#define PRM_APF_CFREQ           'APFC'
#define PRM_APF_ORDER           'APFO'
#define PRM_APF_FORM            'APFF'
#define PRM_CROSSOVER_F0_ID     'CRF0'
#define PRM_CROSSOVER_F1_ID     'CRF1'
#define PRM_CROSSOVER_F2_ID     'CRF2'
#define PRM_CROSSOVER_G1_ID     'CRG1'
#define PRM_CROSSOVER_G2_ID     'CRG2'
#define PRM_CROSSOVER_G3_ID     'CRG3'
#define PRM_CROSSOVER_G4_ID     'CRG4'
#define PRM_CROSSOVER_FORM1     'CFO1'
#define PRM_CROSSOVER_FORM2     'CFO2'

#define LPF         0
#define HPF         1
#define BPF         2
#define BSF         3
#define PQE         4
#define LSH         5
#define HSH         6
#define APF         7
#define OFF         8

#pragma pack(push,1)
typedef struct id_s{
    uint8_t empty;  //
    uint8_t effect;
    uint8_t sub_effect;
    uint8_t prm;
}id_t;
#pragma pack(pop)

union id_union_t{
    int32_t         all;
    id_t            id;
};

//Efect ID
#define EQ1                   0

//Efect ID
#define Compresor_nb      1
//Sub efect
#define COMPRESOR_1b            0
#define COMPRESOR_2b            1
#define COMPRESOR_3b            2
#define COMPRESOR_4b            3
#define Crossover               4
#define Enable_id               255

#define EQ2                     2


#define Limiter                 3

#define Multy_Band_Compresor    1

#endif
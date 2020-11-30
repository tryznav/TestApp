#ifndef __SET_PARAMS_H__
#define __SET_PARAMS_H__

#include "stdint.h"

#pragma pack(push,1)
typedef struct id_s{
    uint8_t empty;  //
    uint8_t effect;
    uint8_t sub_effect;
    uint8_t prm;
}id_t;
#pragma pack(pop)

typedef union id_union{
    int32_t         all;
    id_t            id;
}id_union_t;

#endif

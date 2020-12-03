#ifndef __SET_PARAMS_H__
#define __SET_PARAMS_H__

#include "stdint.h"

#pragma pack(push,1)
typedef union id_union{
    struct{
        uint8_t empty;  //
        uint8_t effect;
        uint8_t sub_effect;
        uint8_t prm;
    };
    int32_t            id;
}id_union_t;
#pragma pack(pop)
#endif

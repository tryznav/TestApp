#include "filters.h"
#include <stdlib.h>

/******************************************************************************/

void apf2_band2_process_get_sizes(
    size_t*     states_bytes){

    *states_bytes = sizeof(biquad_cascade4_states_t);

}

/*******************************************************************************/

void apf2_band2_reset(
    apf2_band2_coeffs_t const*   coeffs,
    apf2_band2_states_t*         states){

    for(uint32_t i = 0; i < FOUR; i++){
        states->x1.ch[i] = 0.0f;
        states->x2.ch[i] = 0.0f;
    }
}

/******************************************************************************/

// void apf2_band2_process(
//     apf2_band2_coeffs_t*   coeffs,
//     apf2_band2_states_t*   states,
//     stereo_t*                   frame){ //frame 
//     vfloat4_t x;
//     vfloat4_t y;

//     for(uint32_t i = 0; i < FRAME_SIZE; i++){
//         x.ch[0] = frame[i].Left;
//         x.ch[1] = frame[i].Right;

//         y = v4_mac(coeffs->b0, x, states->x1);

//         x.ch[2] = y.ch[0];
//         x.ch[3] = y.ch[1];

//         y = v4_mac(coeffs->b0, x, states->x1);

//         states->x1 = v4_mac(coeffs->b1, x, states->x2);
//         states->x1 = v4_mac(coeffs->a1, y, states->x1);

//         states->x2 = v4_mul(coeffs->b2, x);
//         states->x2 = v4_mac(coeffs->a2, y, states->x2);

//         frame[i].Left = y.ch[2];
//         frame[i].Right = y.ch[3];

//     }

// }

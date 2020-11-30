#include "filters.h"
#include <stdlib.h>

/******************************************************************************/

void biquad_cascade4_process_get_sizes(
    size_t*     states_bytes){

    *states_bytes = sizeof(biquad_cascade4_states_t);

}

/*******************************************************************************/

void biquad_cascade4_reset(
    biquad_cascade4_coeffs_t*   coeffs,
    biquad_cascade4_states_t*   states){

    for(uint32_t i = 0; i < EIGHT; i++){
        states->x1.ch[i] = 0.0f;
        states->x2.ch[i] = 0.0f;
    }
}

/******************************************************************************/

// void biquad_cascade4_process(
//     biquad_cascade4_coeffs_t*   coeffs,
//     biquad_cascade4_states_t*   states,
//     stereo_t*                   frame){ //frame
//     vfloat8_t x;
//     vfloat8_t y;

//     for(uint32_t i = 0; i < FRAME_SIZE; i++){
//         x.ch[0] = frame[i].Left;
//         x.ch[1] = frame[i].Right;

//         y = v8_mac(coeffs->b0, x, states->x1);

//         x.ch[2] = y.ch[0];
//         x.ch[3] = y.ch[1];

//         y = v8_mac(coeffs->b0, x, states->x1);
//         x.ch[4] = y.ch[2];
//         x.ch[5] = y.ch[3];

//         y = v8_mac(coeffs->b0, x, states->x1);
//         x.ch[6] = y.ch[4];
//         x.ch[7] = y.ch[5];

//         y = v8_mac(coeffs->b0, x, states->x1);

//         states->x1 = v8_mac(coeffs->b1, x, states->x2);
//         states->x1 = v8_mac(coeffs->a1,  y, states->x1);

//         states->x2 = v8_mul(coeffs->b2, x);
//         states->x2 = v8_mac(coeffs->a2,  y, states->x2);

//         frame[i].Left = y.ch[6];
//         frame[i].Right = y.ch[7];
//     }
// }

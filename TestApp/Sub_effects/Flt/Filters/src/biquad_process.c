#include "filters.h"
#include <stdlib.h>

/******************************************************************************/

void biquad_process_get_sizes(
    size_t*     states_bytes){
    *states_bytes = sizeof(biquad_states_t);
}

/*******************************************************************************/

void biquad_reset(
    biquad_coeffs_t*   coeffs,
    biquad_states_t*   states){

    for(uint32_t i = 0; i < 2; i++){
        states->x1.ch[i] = 0.0f;
        states->x2.ch[i] = 0.0f;
    }
}

/******************************************************************************/

// void biquad_process(
//     biquad_coeffs_t *   coeffs,
//     biquad_states_t*   states,
//     stereo_t*                   frame){ //frame 
//     vfloat4_t x;
//     vfloat4_t y;
//     x.ch[2] = 0.0f;
//     x.ch[3] = 0.0f;

//     for(uint32_t i = 0; i < FRAME_SIZE; i++){
//         x.ch[0] = frame[i].Left;
//         x.ch[1] = frame[i].Right;

//         y = v4_mac(coeffs->b0, x, states->x1);

//         states->x1 = v4_mac(coeffs->b1, x, states->x2);
//         states->x1 = v4_mac(coeffs->a1, y, states->x1);

//         states->x2 = v4_mul(coeffs->b2, x);
//         states->x2 = v4_mac(coeffs->a2, y, states->x2);

//         frame[i].Left = y.ch[0];
//         frame[i].Right = y.ch[1];

//     }
// }

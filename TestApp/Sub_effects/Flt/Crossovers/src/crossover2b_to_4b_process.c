#include "crossovers.h"
#include <stdlib.h>

/******************************************************************************/

void crossover2b_to_4b_process_get_sizes(
    size_t*     states_bytes){

    *states_bytes = sizeof(crossover2b_to_4b_states_t);

}

/*******************************************************************************/

void crossover2b_to_4b_reset(
    crossover2b_to_4b_coeffs_t const*   coeffs,
    crossover2b_to_4b_states_t*         states){

    for(uint32_t i = 0; i < FOUR; i++){
        states->xh.ch[i] = 0.0f;
        states->xh1.ch[i] = 0.0f;
        states->xh2.ch[i] = 0.0f;
    }
    for(uint32_t i = 0; i < FRAME_SIZE; i++){
        for(uint32_t c = 0; c < EIGHT; c++){
            states->bands4[i].ch[c] = 0.0f;
            states->bands4[i].ch[c] = 0.0f;
        }
    }
}

/******************************************************************************/

// void biquad_cascade2_process(
//     biquad_cascade2_coeffs_t*   coeffs,
//     biquad_cascade2_states_t*   states,
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

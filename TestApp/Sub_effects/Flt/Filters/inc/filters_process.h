#ifndef __FILTERS_PROCESS_H__
#define __FILTERS_PROCESS_H__

#include <stdint.h>
#include <stddef.h>

#include "filters.h"
#include "filters.h"
#include <stdlib.h>


/******************************************************************************/

void biquad_process_get_sizes(
    size_t*     states_bytes);

void biquad_reset(
    biquad_coeffs_t const*      coeffs,
    biquad_states_t*            states);

static inline void biquad_process(
    biquad_coeffs_t const*      coeffs,
    biquad_states_t*            states,
    stereo_t*                   frame){ //frame 
    vfloat4_t x;
    vfloat4_t y;
    x.ch[2] = 0.0f;
    x.ch[3] = 0.0f;

    for(uint32_t i = 0; i < FRAME_SIZE; i++){
        x.ch[0] = frame[i].Left;
        x.ch[1] = frame[i].Right;

        y = v4_mac(coeffs->b0, x, states->x1);

        states->x1 = v4_mac(coeffs->b1, x, states->x2);
        states->x1 = v4_mac(coeffs->a1, y, states->x1);

        states->x2 = v4_mul(coeffs->b2, x);
        states->x2 = v4_mac(coeffs->a2, y, states->x2);

        frame[i].Left = y.ch[0];
        frame[i].Right = y.ch[1];

    }
}

/******************************************************************************/

void biquad_cascade2_process_get_sizes(
    size_t*     states_bytes);

void biquad_cascade2_reset(
    biquad_cascade2_coeffs_t const*     coeffs,
    biquad_cascade2_states_t*           states);

static inline void biquad_cascade2_process(
    biquad_cascade2_coeffs_t const*     coeffs,
    biquad_cascade2_states_t*           states,
    stereo_t*                           frame){ //frame 
    vfloat4_t x;
    vfloat4_t y;

    for(uint32_t i = 0; i < FRAME_SIZE; i++){
        x.ch[0] = frame[i].Left;
        x.ch[1] = frame[i].Right;

        y = v4_mac(coeffs->b0, x, states->x1);

        x.ch[2] = y.ch[0];
        x.ch[3] = y.ch[1];

        y = v4_mac(coeffs->b0, x, states->x1);

        states->x1 = v4_mac(coeffs->b1, x, states->x2);
        states->x1 = v4_mac(coeffs->a1, y, states->x1);

        states->x2 = v4_mul(coeffs->b2, x);
        states->x2 = v4_mac(coeffs->a2, y, states->x2);

        frame[i].Left = y.ch[2];
        frame[i].Right = y.ch[3];

    }

}
/***********************************************************/

void biquad_cascade4_process_get_sizes(
    size_t*     states_bytes);


void biquad_cascade4_reset(
    biquad_cascade4_coeffs_t const*   coeffs,
    biquad_cascade4_states_t*   states);

static inline void biquad_cascade4_process(
    biquad_cascade4_coeffs_t const*     coeffs,
    biquad_cascade4_states_t*           states,
    stereo_t*                           frame){ //frame
    vfloat8_t x;
    vfloat8_t y;

    for(uint32_t i = 0; i < FRAME_SIZE; i++){
        x.ch[0] = frame[i].Left;
        x.ch[1] = frame[i].Right;

        y = v8_mac(coeffs->b0, x, states->x1);

        x.ch[2] = y.ch[0];
        x.ch[3] = y.ch[1];

        y = v8_mac(coeffs->b0, x, states->x1);
        x.ch[4] = y.ch[2];
        x.ch[5] = y.ch[3];

        y = v8_mac(coeffs->b0, x, states->x1);
        x.ch[6] = y.ch[4];
        x.ch[7] = y.ch[5];

        y = v8_mac(coeffs->b0, x, states->x1);

        states->x1 = v8_mac(coeffs->b1, x, states->x2);
        states->x1 = v8_mac(coeffs->a1,  y, states->x1);

        states->x2 = v8_mul(coeffs->b2, x);
        states->x2 = v8_mac(coeffs->a2,  y, states->x2);

        frame[i].Left = y.ch[6];
        frame[i].Right = y.ch[7];;
    }
}

/********************************************************/
void apf2_band2_process_get_sizes(
    size_t*     states_bytes);

void apf2_band2_reset(
    apf2_band2_coeffs_t const*      coeffs,
    apf2_band2_states_t*            states);

static inline void apf2_band2_process(
    apf2_band2_coeffs_t const*      coeffs,
    apf2_band2_states_t*            states,
    band2_t*                        frame){ //frame
    vfloat4_t*  x = (vfloat4_t*)frame;
    vfloat4_t y;

    for(uint32_t i = 0; i < FRAME_SIZE; i++){
        y = v4_mac(coeffs->k2, x[i], states->x1);

        states->x1 = v4_msub(coeffs->k1,  y, states->x2);
        states->x1 =  v4_mac(coeffs->k1, x[i], states->x1);

        states->x2 = v4_msub(coeffs->k2, y, x[i]);

        x[i] = y;
    }
}
/*******************************************************/


#endif
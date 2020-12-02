#ifndef __CROSSOVERS_PROCESS_H__
#define __CROSSOVERS_PROCESS_H__

#include <stdint.h>
#include <stddef.h>

#include "crossovers.h"
#include "filters_process.h"
#include <stdlib.h>

/****************************************************************/
void crossover2b_process_get_sizes(
    size_t*     states_bytes);

void crossover2b_reset(
    crossover2b_coeffs_t const*     coeffs,
    crossover2b_states_t*           states);

static inline void crossover2b_process(
    crossover2b_coeffs_t const*     coeffs,
    crossover2b_states_t*           states,
    stereo_t*                       frame){ //frame
    vfloat4_t x;
    x.ch[2] = 0.0f;
    x.ch[3] = 0.0f;//warning!!! Ask Yury about it
    vfloat4_t f;
    vfloat4_t y_2nd;
    vfloat4_t y_1st;

    for(uint32_t i = 0; i < FRAME_SIZE; i++){
        x.ch[0] = frame[i].Left;
        x.ch[1] = frame[i].Right;

        x           = v4_mul(x,          coeffs->half);
        y_2nd       = v4_mac(coeffs->k2, x,         states->xh1);

        states->xh1 = v4_msub(coeffs->k1, y_2nd,    states->xh2);
        states->xh1 = v4_mac (coeffs->k1, x,        states->xh1);

        states->xh2 = v4_msub(coeffs->k2, y_2nd,    x);
        /*******************************************************/
        //first order
        f           = v4_msub(coeffs->k, states->xh,x);
        y_1st       = v4_mac (coeffs->k, f,         states->xh);

        states->xh = f;
        /*******************************************************/
        //get low
        f = v4_neg(y_1st);
        y_1st.ch[2] = f.ch[0];
        y_1st.ch[3] = f.ch[1];
        y_2nd.ch[2] = y_2nd.ch[0];
        y_2nd.ch[3] = y_2nd.ch[1];
    
        // states->bands2[i] = y_1st;

        states->bands2[i] = v4_add(y_2nd, y_1st);
    }
}

/****************************************************************/
void crossover2b_to_4b_process_get_sizes(
    size_t*     states_bytes);

void crossover2b_to_4b_reset(
    crossover2b_to_4b_coeffs_t const*     coeffs,
    crossover2b_to_4b_states_t*           states);

static inline void crossover2b_to_4b_process(
    crossover2b_to_4b_coeffs_t const*     coeffs,
    crossover2b_to_4b_states_t*           states,
    band2_t*                              frame){ //frame
    vfloat4_t *x = (vfloat4_t *)frame;
    vfloat4_t f;
    vfloat4_t y_2nd;
    vfloat4_t y_1st;

    for(uint32_t i = 0; i < FRAME_SIZE; i++){
        x[i]        = v4_mul(x[i],       coeffs->half);
        y_2nd       = v4_mac(coeffs->k2, x[i],      states->xh1);

        states->xh1 = v4_msub(coeffs->k1, y_2nd,    states->xh2);
        states->xh1 = v4_mac (coeffs->k1, x[i],     states->xh1);

        states->xh2 = v4_msub(coeffs->k2, y_2nd,    x[i]);
        /*******************************************************/
        //first order
        f           = v4_msub(coeffs->k, states->xh, x[i]);
        y_1st       = v4_mac (coeffs->k, f,         states->xh);

        states->xh = f;
        /*******************************************************/
        //get low
        f = v4_add(y_2nd, y_1st); //LOW part
        for (uint32_t c = 0; c < FOUR; c++)
        {
            states->bands4[i].ch[c] = f.ch[c];
        }
        f = v4_sub(y_2nd, y_1st);
        for (uint32_t c = 0; c < FOUR; c++)
        {
            states->bands4[i].ch[c + 4] = f.ch[c];
        }

    }
}

/****************************************************************/
void crossover4b_process_get_sizes(
    size_t*     states_bytes);

void crossover4b_reset(
    crossover4b_coeffs_t const*     coeffs,
    crossover4b_states_t*           states);

static inline void crossover4b_process(
    crossover4b_coeffs_t const*     coeffs,
    crossover4b_states_t*           states,
    stereo_t*                              frame){ //frame
    crossover2b_process((crossover2b_coeffs_t const*)&coeffs->crossover2b, &states->crossover2b, frame);

    apf2_band2_process((apf2_band2_coeffs_t const*)&coeffs->apf_compens,&states->apf_compens, (band2_t *)states->crossover2b.bands2);

    crossover2b_to_4b_process((crossover2b_to_4b_coeffs_t const*)&coeffs->crossover2b_to_4b, &states->crossover2b_to_4b, (band2_t *)states->crossover2b.bands2);
}
#endif
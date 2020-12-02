#ifndef __COMPRESSORS_PROCESS_H__
#define __COMPRESSORS_PROCESS_H__

#include <stdint.h>
#include <stddef.h>

#include "compressors.h"
#include "crossovers_process.h"


/******************************************************************************/

void compressor_4band_process_get_sizes(
    size_t*                     states_bytes);

void compressor_4band_reset(
    compressor_4band_coeffs_t const*      coeffs,
    compressor_4band_states_t*            states);

static inline void smooth_4band(vfloat8_t *env, vfloat8_t *prev, vfloat8_t *ar, vfloat8_t *ar_m, vfloat8_t *x){
    *env = v8_mul(*prev, *ar);
    *env = v8_mac(*ar_m, *x, *env);
}

static inline void compressor_4band_process(
    compressor_4band_coeffs_t const*      coeffs,
    compressor_4band_states_t*            states,
    stereo_t*                             frame){ //frame

    crossover4b_process((crossover4b_coeffs_t const*)&coeffs->crossover, &states->crossover, frame);

    for(uint32_t i = 0; i < FRAME_SIZE; i++){
        vfloat8_t x_abs = v8_abs(states->crossover.crossover2b_to_4b.bands4[i]);

        vfloat8_t ONE;
        for (size_t i = 0; i < EIGHT; i++)
        {
            ONE.ch[i] = 1.0f;
        }

        vfloat8_t env;
        vfloat8_t gain_tmp;

        vfloat8_t ar;
        vfloat8_t ar_m;

        vfloat8_t mask = v8_cpm_gt(x_abs, states->env);

        ar = v8_blend(coeffs->e_release, coeffs->e_attack, mask);
        ar_m = v8_blend(coeffs->e_release_m, coeffs->e_attack_m, mask);
        
        smooth_4band(&env, &states->env, &ar, &ar_m, &x_abs);

        states->env = env;

        mask = v8_cpm_gt(env, coeffs->threshold);

        gain_tmp = v8_div(  v8_mul(coeffs->threshold,
                            v8_pow( v8_div(states->env, coeffs->threshold),
                                    v8_div(ONE, coeffs->ratio))), states->env);

        gain_tmp = v8_blend(ONE, gain_tmp, mask);

        mask =  v8_cpm_gt(states->g_s, gain_tmp);

        ar =    v8_blend(coeffs->a_release, coeffs->a_attack, mask);
        ar_m =  v8_blend(coeffs->a_release_m, coeffs->a_attack_m, mask);

        smooth_4band(&env, &states->g_s, &ar, &ar_m, &gain_tmp);
        states->g_s = env;
        

        // //Make up gain calculate
        gain_tmp = v8_mul(states->g_s, coeffs->makeUpGain);

        states->crossover.crossover2b_to_4b.bands4[i] = v8_mul(states->crossover.crossover2b_to_4b.bands4[i], gain_tmp);
    
        frame[i].Left =     states->crossover.crossover2b_to_4b.bands4[i].ch[0] + 
                            states->crossover.crossover2b_to_4b.bands4[i].ch[2] +
                            states->crossover.crossover2b_to_4b.bands4[i].ch[4] +
                            states->crossover.crossover2b_to_4b.bands4[i].ch[6];

        frame[i].Right =    states->crossover.crossover2b_to_4b.bands4[i].ch[1] + 
                            states->crossover.crossover2b_to_4b.bands4[i].ch[3] +
                            states->crossover.crossover2b_to_4b.bands4[i].ch[5] +
                            states->crossover.crossover2b_to_4b.bands4[i].ch[7];
        
    }
}

/******************************************************************************/
#endif
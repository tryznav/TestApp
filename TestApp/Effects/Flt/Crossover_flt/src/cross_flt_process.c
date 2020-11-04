#include "cross_flt.h"
#include <stdlib.h>
#ifdef WIN32
    #include <Windows.h>
#else
    #include <unistd.h>    
#endif

/******************************************************************************/

int32_t cross_flt_process_get_sizes(
    size_t*     states_bytes){

    *states_bytes = sizeof(states_t);

    return 0;
}

/*******************************************************************************/

int32_t cross_flt_reset(
    void const* coeffs,
    void*       states){

    states_t *_st = states;
    for(int i = 0; i < (NUM_OF_FREQ * 2); i++){
        for(int o = 0; o < 2; o++){
            _st->Left.apf_1st[i].xh[o] = 0.0f;
            _st->Left.apf_2nd[i].xh[o] = 0.0f;
            _st->Right.apf_1st[i].xh[o] = 0.0f;
            _st->Right.apf_2nd[i].xh[o] = 0.0f;
        }
    }
    return 0;
}

/******************************************************************************/
static int32_t check(void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count)
{
    if(audio == NULL){
        fprintf(stderr, RED "Error:\t"RESET BOLDWHITE"Wrong 'audio' pointer. Rejected\n"RESET);
        return -1;
    }

    if(states == NULL){
        fprintf(stderr, RED "Error:\t"RESET BOLDWHITE"Wrong 'states' pointer. Rejected\n"RESET);
        return -1;
    }

    if(coeffs == NULL){
        fprintf(stderr, RED "Error:\t"RESET BOLDWHITE"Wrong 'coeffs' pointer. Rejected\n"RESET);
        return -1;
    }
    
    if(samples_count == 0){
        fprintf(stderr, RED "Error:\t"RESET BOLDWHITE"Wrong 'coeffs' pointer. Rejected\n"RESET);
        return -1;
    }

    return 0;
}

static audio_type apl_1st_order(audio_type x, apf_states_t *st, apf_coef_t *coef){
    audio_type xh = flt_sub(x, flt_mul(coef->k[0], st->xh[0]));

    audio_type y =  flt_mac(st->xh[0], xh, coef->k[0]);

    st->xh[0] = xh;

    return y;
}

static audio_type apl_2nd_order(audio_type x, apf_states_t *st, apf_coef_t *coef){
    audio_type  xh = flt_msub(x, coef->k[1], st->xh[1]);
    audio_type  y = flt_mac(st->xh[1], coef->k[1], xh);

    st->xh[1] = apl_1st_order(xh, st, coef);

    return y;
}

static audio_type calc_crossover(audio_type x, cross_states_t *st, coef_t *coef){
    audio_type band1 = 0.0f;
    audio_type band2 = 0.0f;
    audio_type band3 = 0.0f;
    audio_type band4 = 0.0f;

    //calculate for f2
    audio_type  apl_2nd = apl_2nd_order(x,       &(st->apf_2nd[0]), &coef->apf_2nd[1]);
    audio_type  apl_1st = apl_1st_order(x,       &(st->apf_1st[0]), &coef->apf_1st[1]);
    audio_type  l_f2 = flt_add(apl_2nd, apl_1st) * 0.5f;

        apl_2nd = apl_2nd_order(l_f2, &(st->apf_2nd[3]), &coef->apf_2nd[1]);
        apl_1st = apl_1st_order(l_f2, &(st->apf_1st[3]), &coef->apf_1st[1]);     
        
        l_f2 = flt_add(apl_2nd, apl_1st) * 0.5f;
    audio_type l = l_f2 * 0.5f;
    audio_type h_f2 = x - l_f2;
    audio_type h = h_f2 * 0.5f;

    // audio_type apl_1nd2 = apl_2nd_order(x, &(cr->xh_apf_2ord_h[1]), &coef->apf_2nd[1]); 

    apl_2nd = apl_2nd_order(l_f2, &(st->apf_2nd[1]), &coef->apf_2nd[0]);   
    apl_1st = apl_1st_order(l_f2, &(st->apf_1st[1]), &coef->apf_1st[0]);

    band1 = flt_add(apl_2nd, apl_1st) * 0.5f;

        apl_2nd = apl_2nd_order(band1, &(st->apf_2nd[4]), &coef->apf_2nd[1]);
        apl_1st = apl_1st_order(band1, &(st->apf_1st[4]), &coef->apf_1st[1]);

    band1 = flt_add(apl_2nd, apl_1st) * 0.5f;

    band2 = l_f2 - band1;//flt_sub(apl_2nd, apl_1st) * 0.5f;

    // h_f2 = apl_2nd_order(h_f2, &(st->apf_1st[2]), &coef->apf_2nd[0]);

    // apl_1st = apl_1st_order(h_f2, &(cr->xh_apf_1ord[0]), coef->apf_1st[1].c);

    apl_2nd = apl_2nd_order(h_f2, &(st->apf_2nd[2]), &coef->apf_2nd[2]);   
    apl_1st = apl_1st_order(h_f2, &(st->apf_1st[2]), &coef->apf_1st[2]);

    band3 = flt_add(apl_2nd, apl_1st) * 0.5f;

        apl_2nd = apl_2nd_order(band3, &(st->apf_2nd[5]), &coef->apf_2nd[1]);
        apl_1st = apl_1st_order(band3, &(st->apf_1st[5]), &coef->apf_1st[1]);

    band3 = flt_add(apl_2nd, apl_1st) * 0.5f;
    band4 = h_f2 - band3;// flt_sub(apl_2nd, apl_1st) * 0.5f;

    
    // band1 = apl_2nd_order(band1,  &(st->apf_2nd[4]), &coef->apf_2nd[2]);   
        
    // band2 = apl_2nd_order(band2,  &(st->apf_2nd[5]), &coef->apf_2nd[2]);   

    // audio_type l_f23 = flt_add(apl_2nd, apl_1st) * 0.5f;


    audio_type res = 0;
    res = flt_mac(res, band1, coef->gain_c[0]);
    res = flt_mac(res, band2, coef->gain_c[1]);
    res = flt_mac(res, band3, coef->gain_c[2]);
    res = flt_mac(res, band4, coef->gain_c[3]);

    return res;
}


int32_t cross_flt_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count){
    int n = check(coeffs, states, audio, samples_count);
    if(n == -1){
        return -1;
    }

    chanes_t *_audio = (chanes_t *)(audio);
    states_t* _st = (states_t*)states;
    coef_t* coef = (coef_t*)coeffs;

    for(uint32_t a_index = 0; a_index < samples_count; a_index++){
        _audio[a_index].Left = calc_crossover(_audio[a_index].Left, &(_st->Left), coef);
        //_audio[a_index].Right = ;
    }

    return 0;
}



// audio_type calc_canonical_filer(audio_type x, xh_z_t *xh, canon_coefs_t *coef){
//     audio_type  xh_z0 = x;
//     xh_z0 = flt_msub(xh_z0, coef->a1, xh->z_1);
//     xh_z0 = flt_msub(xh_z0, coef->a2, xh->z_2);
//     // printf("xh_z0 %f\n",  xh_z0);
//     // Sleep(100);
//     // exit(EXIT_FAILURE);

//     audio_type  y = flt_mul(coef->b0, xh_z0);
//     y = flt_mac(y, coef->b1, xh->z_1);
//     y = flt_mac(y, coef->b2, xh->z_2);    

//     xh->z_2 = xh->z_1;
//     xh->z_1 = xh_z0;

//     return y;
// }
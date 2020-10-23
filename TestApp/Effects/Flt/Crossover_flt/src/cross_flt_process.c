#include "cross_flt.h"
#include <stdlib.h>
#include <Windows.h>

/******************************************************************************/

int32_t cross_flt_process_get_sizes(
    size_t*     states_bytes){

    *states_bytes = sizeof(cross_states_t);

    return 0;
}

/*******************************************************************************/

int32_t cross_flt_reset(
    void const* coeffs,
    void*       states){

    cross_states_t *_st = states;
    for(int freq_index = 0; freq_index < NUM_OF_FREQ; freq_index++){
        _st->Left.xh_hpf[freq_index].z_1 = 0.0f;
        _st->Left.xh_hpf[freq_index].z_2 = 0.0f;
        _st->Left.xh_lpf[freq_index].z_1 = 0.0f;
        _st->Left.xh_lpf[freq_index].z_2 = 0.0f;
        _st->Left.xh_apf_2ord_ll[freq_index].z_1 = 0.0f;
        _st->Left.xh_apf_2ord_ll[freq_index].z_2 = 0.0f;
        _st->Left.xh_apf_2ord_lh[freq_index].z_1 = 0.0f;
        _st->Left.xh_apf_2ord_lh[freq_index].z_2 = 0.0f;
        _st->Left.xh_apf_2ord_h[freq_index].z_1 = 0.0f;
        _st->Left.xh_apf_2ord_h[freq_index].z_2 = 0.0f;
        _st->Left.xh_apf_2ord[freq_index].z_1 = 0.0f;
        _st->Left.xh_apf_2ord[freq_index].z_2 = 0.0f;
        _st->Left.xh_apf_1ord[freq_index] = 0.0f;
        _st->Right.xh_hpf[freq_index].z_1 = 0.0f;
        _st->Right.xh_hpf[freq_index].z_2 = 0.0f;
        _st->Right.xh_lpf[freq_index].z_1 = 0.0f;
        _st->Right.xh_lpf[freq_index].z_2 = 0.0f;
        _st->Right.xh_apf_2ord_ll[freq_index].z_1 = 0.0f;
        _st->Right.xh_apf_2ord_ll[freq_index].z_2 = 0.0f;
        _st->Right.xh_apf_2ord_lh[freq_index].z_1 = 0.0f;
        _st->Right.xh_apf_2ord_lh[freq_index].z_2 = 0.0f;
        _st->Right.xh_apf_2ord_h[freq_index].z_1 = 0.0f;
        _st->Right.xh_apf_2ord_h[freq_index].z_2 = 0.0f;
        _st->Right.xh_apf_2ord[freq_index].z_1 = 0.0f;
        _st->Right.xh_apf_2ord[freq_index].z_2 = 0.0f;
        _st->Right.xh_apf_1ord[freq_index] = 0.0f;
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


audio_type apl_1st_order(audio_type x, audio_type *xh_1, coef_type c){
    audio_type xh = flt_sub(x, flt_mul(c, *xh_1));

    audio_type y = flt_mul(c, xh);
    y = flt_add(y, *xh_1);

    *xh_1 = xh;
    return y;
}

audio_type apl_2nd_order(audio_type x, xh_z_t *xh, apf_coef_2nd_t *coef){
    audio_type  xh_z0 = x;
    xh_z0 = flt_msub(xh_z0, coef->dc, xh->z_1);
    xh_z0 = flt_mac(xh_z0, coef->c, xh->z_2);
    // Sleep(100);
    // printf("xh_z0 %f\n",  xh_z0);

    audio_type  y = flt_mul(coef->dc, xh->z_1);
    y = flt_msub(y, coef->c, xh_z0);
    y = flt_add(y, xh->z_2);    

    xh->z_2 = xh->z_1;
    xh->z_1 = xh_z0;
    return y;
}

audio_type calc_canonical_filer(audio_type x, xh_z_t *xh, canon_coefs_t *coef){
    audio_type  xh_z0 = x;
    xh_z0 = flt_msub(xh_z0, coef->a1, xh->z_1);
    xh_z0 = flt_msub(xh_z0, coef->a2, xh->z_2);
    // printf("xh_z0 %f\n",  xh_z0);
    // Sleep(100);
    // exit(EXIT_FAILURE);

    audio_type  y = flt_mul(coef->b0, xh_z0);
    y = flt_mac(y, coef->b1, xh->z_1);
    y = flt_mac(y, coef->b2, xh->z_2);    

    xh->z_2 = xh->z_1;
    xh->z_1 = xh_z0;

    return y;
}

audio_type calc_crossover(audio_type x, cross_t *cr, cross_coefs_t *coef){
    audio_type band1 = 0.0f;
    audio_type band2 = 0.0f;
    audio_type band3 = 0.0f;
    audio_type band4 = 0.0f;
    //calculate for f2
    audio_type apl_2nd = apl_2nd_order(x, &(cr->xh_apf_2ord[1]), &coef->apf_2nd[1]);    
    audio_type apl_1st = apl_1st_order(x, &(cr->xh_apf_1ord[1]), coef->apf_1st[1].c);

    audio_type l_f2 = flt_add(apl_2nd, apl_1st) * 0.5f;
    audio_type h_f2 = flt_sub(apl_2nd, apl_1st) * 0.5f;

    // audio_type apl_1nd2 = apl_2nd_order(x, &(cr->xh_apf_2ord_h[1]), &coef->apf_2nd[1]); 
    apl_2nd = apl_2nd_order(l_f2, &(cr->xh_apf_2ord[0]), &coef->apf_2nd[0]);    
    apl_1st = apl_1st_order(l_f2, &(cr->xh_apf_1ord[0]), coef->apf_1st[0].c);

    band1 = flt_add(apl_2nd, apl_1st) * 0.5f;
    band2 = flt_sub(apl_2nd, apl_1st) * 0.5f;

    h_f2 = apl_2nd_order(h_f2, &(cr->xh_apf_2ord_h[0]), &coef->apf_2nd[0]);    
    // apl_1st = apl_1st_order(h_f2, &(cr->xh_apf_1ord[0]), coef->apf_1st[1].c);

    apl_2nd = apl_2nd_order(h_f2, &(cr->xh_apf_2ord[2]), &coef->apf_2nd[2]);    
    apl_1st = apl_1st_order(h_f2, &(cr->xh_apf_1ord[2]), coef->apf_1st[2].c);

    band3 = flt_add(apl_2nd, apl_1st) * 0.5f;
    band4 = flt_sub(apl_2nd, apl_1st) * 0.5f;

    
    band1 = apl_2nd_order(band1, &(cr->xh_apf_2ord_ll[2]), &coef->apf_2nd[2]);   
    
    band2 = apl_2nd_order(band2, &(cr->xh_apf_2ord_lh[2]), &coef->apf_2nd[2]);   

    // audio_type l_f23 = flt_add(apl_2nd, apl_1st) * 0.5f;


//     audio_type h_f2 = flt_sub(apl_2nd, apl_1st) * 0.5f;
//     l_f2 = apl_2nd_order(l_f2, &(cr->xh_apf_2ord_l[2]), &coef->apf_2nd[2]);
//     h_f2 = apl_2nd_order(h_f2, &(cr->xh_apf_2ord_h[0]), &coef->apf_2nd[0]);

//     apl_2nd = apl_2nd_order(l_f2, &(cr->xh_apf_2ord[0]), &coef->apf_2nd[0]);
//     apl_1st = apl_1st_order(l_f2, &(cr->xh_apf_1ord[0]), coef->apf_1st[0].c);
    
//     band2 = flt_sub(apl_2nd, apl_1st) * 0.5f;
   
//     // band2 = flt_sub(apl_2nd, apl_1st) * 0.5f;
//     band1 = apl_2nd - band2;
// band2 = -band2;
//     apl_2nd = apl_2nd_order(h_f2, &(cr->xh_apf_2ord[2]), &coef->apf_2nd[2]);
//     apl_1st = apl_1st_order(h_f2, &(cr->xh_apf_1ord[2]), coef->apf_1st[2].c);

//     band3 =  flt_add(apl_2nd, apl_1st) * 0.5f;
//     band4 =  apl_2nd - band3;


//     band4 = apl_2nd_order(band4, &(cr->xh_apf_2ord_h[0]), &coef->apf_2nd[0]);

    // audio_type h_f2 = calc_canonical_filer(x, &(cr->xh_hpf[1]), &coef->hpf[1]);
    // audio_type band3 = calc_canonical_filer(h_f2, &(cr->xh_lpf[2]), &coef->lpf[2]);
    // audio_type band4 = calc_canonical_filer(h_f2, &(cr->xh_hpf[2]), &coef->hpf[2]);
    // audio_type res = band1 + band2 + band3 + band4;//+ band4;// apl_2nd;
    audio_type res = 0;
    res = flt_mac(res, band1, coef->gain_p[0]);
    res = flt_mac(res, band2, coef->gain_p[1]);
    res = flt_mac(res, band3, coef->gain_p[2]);
    res = flt_mac(res, band4, coef->gain_p[3]);

    return  res;
}
audio_type calc_crossover2(audio_type x, cross_t *cr, cross_coefs_t *coef){
    audio_type band1 = 0.0f;
    audio_type band2 = 0.0f;
    audio_type band3 = 0.0f;
    audio_type band4 = 0.0f;
    //calculate for f2
    audio_type apl_2nd = apl_2nd_order(x, &(cr->xh_apf_2ord[1]), &coef->apf_2nd[1]);    
    audio_type apl_1st = apl_1st_order(x, &(cr->xh_apf_1ord[1]), coef->apf_1st[1].c);

//     audio_type l_f2 = flt_add(apl_2nd, apl_1st) * 0.5f;
//     audio_type h_f2 = flt_sub(apl_2nd, apl_1st) * 0.5f;
//     l_f2 = apl_2nd_order(l_f2, &(cr->xh_apf_2ord_l[2]), &coef->apf_2nd[2]);
//     h_f2 = apl_2nd_order(h_f2, &(cr->xh_apf_2ord_h[0]), &coef->apf_2nd[0]);

//     apl_2nd = apl_2nd_order(l_f2, &(cr->xh_apf_2ord[0]), &coef->apf_2nd[0]);
//     apl_1st = apl_1st_order(l_f2, &(cr->xh_apf_1ord[0]), coef->apf_1st[0].c);
    
//     band2 = flt_sub(apl_2nd, apl_1st) * 0.5f;
   
//     // band2 = flt_sub(apl_2nd, apl_1st) * 0.5f;
//     band1 = apl_2nd - band2;
// band2 = -band2;
//     apl_2nd = apl_2nd_order(h_f2, &(cr->xh_apf_2ord[2]), &coef->apf_2nd[2]);
//     apl_1st = apl_1st_order(h_f2, &(cr->xh_apf_1ord[2]), coef->apf_1st[2].c);

//     band3 =  flt_add(apl_2nd, apl_1st) * 0.5f;
//     band4 =  apl_2nd - band3;


//     band4 = apl_2nd_order(band4, &(cr->xh_apf_2ord_h[0]), &coef->apf_2nd[0]);
    band1 = (x + apl_2nd) / 2.0f;
    band2 = (x + apl_1st) / 2.0f;

    // audio_type h_f2 = calc_canonical_filer(x, &(cr->xh_hpf[1]), &coef->hpf[1]);
    // audio_type band3 = calc_canonical_filer(h_f2, &(cr->xh_lpf[2]), &coef->lpf[2]);
    // audio_type band4 = calc_canonical_filer(h_f2, &(cr->xh_hpf[2]), &coef->hpf[2]);
    audio_type res = band2 + band3 + band4;// + band1 + band3;// + band4 ;//+ band4;// apl_2nd;
    // audio_type res = h_f2 + l_f2;
    // res = flt_mac(res, band1, coef->gain_p[0]);
    // res = flt_mac(res, band2, coef->gain_p[1]);
    // res = flt_mac(res, band3, coef->gain_p[2]);
    // res = flt_mac(res, band4, coef->gain_p[3]);

    return  x;
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
    cross_states_t* _st = (cross_states_t*)states;
    cross_coefs_t* coef = (cross_coefs_t*) coeffs;

    for(uint32_t a_index = 0; a_index < samples_count; a_index++){
        _audio[a_index].Left = calc_crossover(_audio[a_index].Left, &(_st->Left), coef);
        _audio[a_index].Right = calc_crossover2(_audio[a_index].Right, &(_st->Right), coef);
    }

    return 0;
}

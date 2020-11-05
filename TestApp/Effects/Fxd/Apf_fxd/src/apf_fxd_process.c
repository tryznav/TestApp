#include "apf_fxd.h"
#include <stdlib.h>
#ifdef WIN32
    #include <Windows.h>
#else
    #include <unistd.h>    
#endif

#define DIRECT2_FBITS   (COEF_FR - 27) //28
#define FISRT_FBITS     (COEF_FR - 14)


/******************************************************************************/

int32_t apf_fxd_process_get_sizes(
    size_t*     states_bytes){

    *states_bytes = sizeof(states_t);

    return 0;
}

/*******************************************************************************/

int32_t apf_fxd_reset(
    void const* coeffs,
    void*       states){

    states_t *_st = (states_t *)states;
    for(int order = (APF_ORDER - 1); order >= 0 ; order--){
        _st->Left.xh[order] = 0;
        _st->Right.xh[order] = 0;
        _st->Left.xh_dbl[order] = 0.0;
        _st->Right.xh_dbl[order] = 0.0;
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

static audio_type apl_direct_f1_2nd(audio_type x, apf_states_t *st, apf_coefs_t *coef){
    acum_type  acum = 0;
    acum = fxd_mac_m(acum, coef->c[0], st->xh[0]);
    acum = fxd_mac_m(acum, coef->c[1], fxd_rshift(x, 1));
    acum = fxd63_add(acum, fxd63_lshift(st->xh[1], COEF_FR));

    acum = fxd_msub_m(acum, coef->c[1], st->y[1]);
    acum = fxd_msub_m(acum, coef->c[0], st->y[0]);


    st->xh[1] = st->xh[0];
    st->xh[0] = fxd_rshift(x, 1);

    st->y[1] = st->y[0];
    st->y[0] = saturation(fxd63_rshift(acum, (COEF_FR )));

    // (audio_type)acum;

    acum = fxd63_rshift(acum, (COEF_FR - 1));
    acum = saturation(acum);
    // printf("%d \n", st->y[0]);
    return (audio_type)acum;
}

static audio_type apl_direct_f1_t_2nd(audio_type x, apf_states_t *st, apf_coefs_t *coef){
    audio_type xh = fxd_add(x, st->xh[0]);

    st->xh[0] = flt_msub(st->xh[1], coef->c[0], xh);
    st->xh[1] = flt_neg(flt_mul(xh, coef->c[1]));

    audio_type y = flt_mac(st->y[0], coef->c[1], xh);

    st->y[0] = flt_mac(st->y[1], coef->c[0], xh);
    st->y[1] = xh;
    
    return y;
}

static audio_type apl_1st_order(audio_type x, apf_states_t *st, apf_coefs_t *coef){
    acum_type xh = fxd63_lshift(x, FISRT_FBITS ); //convert to type type xh
    // xh = fxd63_add(xh,st->noise[0]);

    xh = fxd_msub_m(xh, coef->c[0], st->xh[0]);

    st->noise[0] = (fxd_q31_t)(xh & ((1u << COEF_FR) - 1));
    xh = fxd63_rshift(xh, COEF_FR); //convert to type type Q30

    

    saturation(xh);

    acum_type y = fxd63_lshift(st->xh[0], COEF_FR);
    y = fxd_mac_m(y, coef->c[0], (audio_type)xh);
    // y = fxd63_add(y, st->noise[1]);
    st->noise[1] = (fxd_q31_t)(y & ((1u << FISRT_FBITS ) - 1));
    y = fxd63_rshift(y, FISRT_FBITS );    //convert to type type Q31

    y = saturation(y);

    st->xh[0] = (audio_type)xh;

    return  (audio_type)y;
}

static audio_type apl_direct_dbl(audio_type x, apf_states_t *st, apf_coefs_t *coef){
    double xh = fxd_to_dbl(x);

    xh -= (coef->c_dbl[0] * st->xh_dbl[0]);
    xh -= (coef->c_dbl[1] * st->xh_dbl[1]);

    double y = st->xh_dbl[1];
    y += coef->c_dbl[1] * xh;
    y += coef->c_dbl[0] * st->xh_dbl[0];

    st->xh_dbl[1] = st->xh_dbl[0];
    st->xh_dbl[0] = xh;

    return (audio_type)dbl_to_fxd(y);
}


static audio_type apl_direct_2nd_order(audio_type x, apf_states_t *st, apf_coefs_t *coef){
    acum_type xh = fxd63_lshift(x, DIRECT2_FBITS); //convert to type type 'xh'
    //  xh = fxd63_add(xh,st->noise[0]);

    xh = fxd_msub_m(xh, coef->c[1], st->xh[0]);
    xh = fxd_mac_m(xh, coef->c[0], st->xh[1]);


    st->noise[0] = (fxd_q31_t)(xh & ((1u << COEF_FR) - 1));
    xh = fxd63_rshift(xh, COEF_FR);

    xh = saturation(xh);


    acum_type y = fxd63_lshift(st->xh[1], COEF_FR);
    y = fxd_mac_m(y, coef->c[1], st->xh[0]);
    //  y = fxd63_add(y,st->noise[1]);
    y = fxd_msub_m(y, coef->c[0], (audio_type)xh);
    st->noise[1] = (fxd_q31_t)(y & ((1u << DIRECT2_FBITS) - 1));

    y = fxd63_rshift(y, DIRECT2_FBITS);                   //convert to type type 'xh'
   
    y = saturation(y);


    st->xh[1] = st->xh[0];
    st->xh[0] = (audio_type)xh;

    return (audio_type)y;
}

static audio_type apl_1st_m_dbl(audio_type x, apf_states_t *st, apf_coefs_t *coef){
    double xh = fxd_to_dbl(x);
        // printf("xh = %f\n", xh);
    xh -= coef->c_dbl[0] * st->xh_dbl[0];

    double y = st->xh_dbl[0] + (xh * coef->c_dbl[0]);

    st->xh_dbl[0] = xh;
    // printf("y = %f\n", y);
    //     // printf("coef->c_dbl[1] = %f\n", coef->c_dbl[1]);
    //     printf("coef->c_dbl[0] = %f\n", coef->c_dbl[0]);
    return dbl_to_fxd(y);
}

static double apl_1st_dbl(double x, apf_states_t *st, apf_coefs_t *coef){
    double xh = x;
    xh -= coef->c_dbl[0] * st->xh_dbl[0];

    double y = st->xh_dbl[0] + (xh * coef->c_dbl[0]);

    st->xh_dbl[0] = xh;

    return y;
}

static audio_type apl_lattice_2nd_order_dbl(audio_type x, apf_states_t *st, apf_coefs_t *coef){
    double xh = fxd_to_dbl(x);

    xh -= coef->c_dbl[1] *  st->xh_dbl[1];
    double y = st->xh_dbl[1] + (coef->c_dbl[1] * xh);

    st->xh_dbl[1] = apl_1st_dbl(xh, st, coef);

    return  dbl_to_fxd(y);
}

static audio_type apl_lattice_2nd_order(audio_type x, apf_states_t *st, apf_coefs_t *coef){
    acum_type p = 0;
    acum_type xh = fxd63_lshift(x, FISRT_FBITS);
    // xh = fxd63_add(xh,st->noise[2]);
    xh = fxd_msub_m(xh, coef->c[1], st->xh[1]);

    st->noise[2] = (fxd_q31_t)(xh & ((1u << COEF_FR) - 1));
    
    xh = fxd63_rshift(xh, COEF_FR);         //convert xh type
    p = xh;
    saturation(xh);
    if(p != xh){
        printf("problem\n");
    }

    acum_type  y = fxd63_lshift(st->xh[1], COEF_FR);
    // y = fxd63_add(y, st->noise[3]);
    y = fxd_mac_m(y, coef->c[1], (audio_type)xh);

    st->noise[3] = (fxd_q31_t)(y & ((1u << FISRT_FBITS ) - 1));
    y = fxd63_rshift(y, FISRT_FBITS );
    
    y = saturation(y);

    st->xh[1] = apl_1st_order((audio_type)xh, st, coef); //type Q31

    return (audio_type)y;
}





int32_t apf_fxd_process(
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
    apf_coefs_t* coef = (apf_coefs_t*)coeffs;

   switch (coef->order)
    {
    case 1:
        for(uint32_t a_index = 0; a_index < samples_count; a_index++){
            _audio[a_index].Left = apl_1st_order(_audio[a_index].Left, &(_st->Left), coef);
            // _audio[a_index].Right = (_audio[a_index].Left -_audio[a_index].Right)*0.5f;//  apf_direct_form2(_audio[a_index].Right, &(_st->Right), coef);
        }
        break;
    case 2:
        switch(coef->form)
        {
        case 1:
            for(uint32_t a_index = 0; a_index < samples_count; a_index++){
                _audio[a_index].Left = apl_direct_f1_2nd(_audio[a_index].Left, &(_st->Left), coef);
                // _audio[a_index].Right = (_audio[a_index].Left, &(_st->Left), coef);
                _audio[a_index].Right = apl_direct_dbl (_audio[a_index].Right, &(_st->Right), coef);//(_audio[a_index].Left +_audio[a_index].Right)*0.5f;//  apf_direct_form2(_audio[a_index].Right, &(_st->Right), coef);
                // _audio[a_index].Right = _audio[a_index].Left - _audio[a_index].Right;
            }
            break;
        case 2:
            for(uint32_t a_index = 0; a_index < samples_count; a_index++){
                // _audio[a_index].Left = apl_direct_f1_t_2nd(_audio[a_index].Left, &(_st->Left), coef);+++++

                // _audio[a_index].Right = (_audio[a_index].Left, &(_st->Left), coef);
                _audio[a_index].Right = apl_direct_dbl (_audio[a_index].Right, &(_st->Right), coef);//(_audio[a_index].Left +_audio[a_index].Right)*0.5f;//  apf_direct_form2(_audio[a_index].Right, &(_st->Right), coef);
                // _audio[a_index].Right = _audio[a_index].Left - _audio[a_index].Right;
            }
            break;
        case 3:
            for(uint32_t a_index = 0; a_index < samples_count; a_index++){
                //_audio[a_index].Left = apl_direct_f2_2nd(_audio[a_index].Left, &(_st->Left), coef);++
                // _audio[a_index].Right = _audio[a_index].Left - _audio[a_index].Right;
                _audio[a_index].Right = apl_direct_dbl (_audio[a_index].Right, &(_st->Right), coef);// (_audio[a_index].Left +_audio[a_index].Right)*0.5f;//  apf_direct_form2(_audio[a_index].Right, &(_st->Right), coef);
                // _audio[a_index].Right = _audio[a_index].Left - _audio[a_index].Right;
            }
            break;
        case 4:
            for(uint32_t a_index = 0; a_index < samples_count; a_index++){
                // _audio[a_index].Left = apl_direct_f2_t_2nd(_audio[a_index].Left, &(_st->Left), coef);++
                // _audio[a_index].Right = _audio[a_index].Left - _audio[a_index].Right;
                _audio[a_index].Right = apl_direct_dbl (_audio[a_index].Right, &(_st->Right), coef);// (_audio[a_index].Left +_audio[a_index].Right)*0.5f;//  apf_direct_form2(_audio[a_index].Right, &(_st->Right), coef);
                // _audio[a_index].Right = _audio[a_index].Left - _audio[a_index].Right;
            }
            break;
        case 5:
            for(uint32_t a_index = 0; a_index < samples_count; a_index++){
                _audio[a_index].Left = apl_lattice_2nd_order(_audio[a_index].Left, &(_st->Left), coef);
                _audio[a_index].Right = apl_direct_dbl(_audio[a_index].Right, &(_st->Right), coef);// (_audio[a_index].Left +_audio[a_index].Right)*0.5f;//  apf_direct_form2(_audio[a_index].Right, &(_st->Right), coef);
                // _audio[a_index].Right = _audio[a_index].Left - _audio[a_index].Right;
            }
            break;
        case 6:
            for(uint32_t a_index = 0; a_index < samples_count; a_index++){
                _audio[a_index].Left = apl_lattice_2nd_order(_audio[a_index].Left, &(_st->Left), coef);
                _audio[a_index].Right = _audio[a_index].Left - _audio[a_index].Right;
                // _audio[a_index].Right = apl_direct_dbl (_audio[a_index].Right, &(_st->Right), coef);// (_audio[a_index].Left +_audio[a_index].Right)*0.5f;//  apf_direct_form2(_audio[a_index].Right, &(_st->Right), coef);
                // _audio[a_index].Right = _audio[a_index].Left - _audio[a_index].Right;
            }
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return 0;
}


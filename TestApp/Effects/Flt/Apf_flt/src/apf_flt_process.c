#include "apf_flt.h"
#include <stdlib.h>
#ifdef WIN32
    #include <Windows.h>
#else
    #include <unistd.h>    
#endif

/******************************************************************************/

int32_t apf_flt_process_get_sizes(
    size_t*     states_bytes){

    *states_bytes = sizeof(states_t);

    return 0;
}

/*******************************************************************************/

int32_t apf_flt_reset(
    void const* coeffs,
    void*       states){

    states_t *_st = (states_t *)states;
    for(int order = (APF_ORDER - 1); order >= 0 ; order--){
        _st->Left.xh[order] = 0.0f;
        _st->Right.xh[order] = 0.0f;
        _st->Right.xh_dbl[order] = 0.0;
        _st->Left.xh_dbl[order] = 0.0;
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

static audio_type apl_1st_order(audio_type x, apf_states_t *st, apf_coefs_t *coef){
    audio_type xh = flt_sub(x, flt_mul(coef->c[0], st->xh[0]));

    audio_type y =  flt_mac(st->xh[0], xh, coef->c[0]);

    st->xh[0] = xh;

    return y;
}

static double apl_1st_dbl(double x, apf_states_t *st, apf_coefs_t *coef){
    double xh = x;
    xh -= coef->c_dbl[0] * st->xh_dbl[0];

    double y = st->xh_dbl[0] + (xh * coef->c_dbl[0]);

    st->xh_dbl[0] = xh;

    return y;
}

static audio_type apl_direct_2nd_order(audio_type x, apf_states_t *st, apf_coefs_t *coef){
    audio_type  xh = x;
    xh = flt_msub(xh, coef->c[1], st->xh[0]);
    xh = flt_mac(xh, coef->c[0], st->xh[1]);

    // printf("xh = %f\n",xh );

    audio_type  y = flt_mul(coef->c[1], st->xh[0]);
    // printf("y = %f\t",y );
    y = flt_msub(y, coef->c[0], xh);
        // printf("y = %f\t",y );
    y = flt_add(y, st->xh[1]);
        // printf("y = %f\n",y );

    st->xh[1] = st->xh[0];
    st->xh[0] = xh;

    return y;
}

static audio_type apl_direct_dbl(audio_type x, apf_states_t *st, apf_coefs_t *coef){
    double xh = x;

    xh -= (coef->c_dbl[1] * st->xh_dbl[0]);
    xh += (coef->c_dbl[0] * st->xh_dbl[1]);
    


    double y = coef->c_dbl[1] * st->xh_dbl[0];
    y -= coef->c_dbl[0] * xh;
    y += st->xh_dbl[1];

    st->xh_dbl[1] = st->xh_dbl[0];
    st->xh_dbl[0] = xh;

    return (audio_type)y;
}

static audio_type apl_lattice_2nd_order(audio_type x, apf_states_t *st, apf_coefs_t *coef){
    audio_type  xh = flt_msub(x, coef->c[1], st->xh[1]);
    audio_type  y = flt_mac(st->xh[1], coef->c[1], xh);

    st->xh[1] = apl_1st_order(xh, st, coef);

    return y;
}

static audio_type apl_lattice_2nd_order_dbl(audio_type x, apf_states_t *st, apf_coefs_t *coef){
    double xh = x;

    xh -= coef->c_dbl[1] *  st->xh_dbl[1];
    double y = st->xh_dbl[1] + (coef->c_dbl[1] * xh);

    st->xh_dbl[1] = apl_1st_dbl(xh, st, coef);

    return (audio_type)y;
}


int32_t apf_flt_process(
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
                _audio[a_index].Left = apl_direct_2nd_order(_audio[a_index].Left, &(_st->Left), coef);
                _audio[a_index].Right = apl_direct_dbl (_audio[a_index].Right, &(_st->Right), coef); //(_audio[a_index].Left +_audio[a_index].Right)*0.5f;//  apf_direct_form2(_audio[a_index].Right, &(_st->Right), coef);
            }
            break;
        case 2:
            for(uint32_t a_index = 0; a_index < samples_count; a_index++){
                _audio[a_index].Left = apl_lattice_2nd_order(_audio[a_index].Left, &(_st->Left), coef);
                _audio[a_index].Right = apl_lattice_2nd_order_dbl(_audio[a_index].Right, &(_st->Right), coef);// (_audio[a_index].Left +_audio[a_index].Right)*0.5f;//  apf_direct_form2(_audio[a_index].Right, &(_st->Right), coef);
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
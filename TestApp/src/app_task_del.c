#include "test_app.h"

int32_t app_gain_del(pross_waw_t *pr){
    if(pr->efect){
        if(pr->efect->params){
            free(pr->efect->params);
            pr->efect->params = NULL;
        }
        if(pr->efect->coeffs){
            free(pr->efect->coeffs);
            pr->efect->coeffs = NULL;
        }
        free(pr->efect);
        pr->efect = NULL;
    }
    return 0;
}

int32_t app_gen_sig_del(pross_waw_t *pr){
    if(pr->gen_sig){
        if(pr->gen_sig->params){
            free(pr->gen_sig->params);
            pr->gen_sig->params = NULL;
        }
        free(pr->gen_sig);
        pr->gen_sig = NULL;
    }
    return 0;
}

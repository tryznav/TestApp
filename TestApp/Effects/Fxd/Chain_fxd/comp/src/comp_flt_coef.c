#include "comp_flt.h"

#ifndef  M_e
    #define M_e    2.71828182846
#endif

static double dB_to_coeff(double dB){
    return pow(10.0f , (0.05f * dB));
}

int32_t comp_flt_coef(comp_prm_t *prm, comp_flt_coef_t *coef){

    coef->threshold =  (coef_type)pow(10.0, (prm->threshold/20.0));  //in linear
    coef->ratio = (coef_type)prm->ratio;
    printf("prm->ratio %f, coef->ratio%f\n", prm->ratio, coef->ratio);

    coef->a_attack = (coef_type)pow(M_e, (-(log(9.0)) / (0.001*prm->tauAttack*prm->samplerate)));
    coef->a_release = (coef_type)pow(M_e, (-(log(9.0)) / (0.001*prm->tauRelease*prm->samplerate)));

    coef->e_attack =(coef_type) pow(M_e, (-(log(9.0)) / (0.001*prm->tauEnvAtt*prm->samplerate)));
    coef->e_release = (coef_type) pow(M_e, (-(log(9.0)) / (0.001*prm->tauEnvRel*prm->samplerate)));


    /*update_coeffs->alphaAttack = 1 - powf(M_e, ((-2.2) / (0.001*update_params->tauAttack*update_params->samplerate)));
    update_coeffs->alphaRelease = 1 - powf(M_e, ((-2.2) / (0.001*update_params->tauRelease*update_params->samplerate)));*/

    //update_coeffs->makeUpGain = update_params->makeUpGain;
    coef->makeUpGain = (coef_type)pow(10.0, (prm->makeUpGain/20.0));

    coef->samplerate = (coef_type)prm->samplerate;
    coef->Enable = prm->Enable;
    printf("coef->e_attack %f\n", coef->e_attack);
    printf("coef->e_release %f\n", coef->e_release);
    printf("coef->a_attack %f\n", coef->a_attack);
    printf("coef->a_release %f\n", coef->a_release);
    printf("coef->makeUpGain  %f\n", coef->makeUpGain);
    printf("prm->samplerate %f\n", prm->samplerate);

    return 0;
}

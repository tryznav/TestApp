#include "comp_fxd.h"

#ifndef  M_e
    #define M_e    2.71828182846
#endif

static double dB_to_coeff(double dB){
    return pow(10.0f , (0.05f * dB));
}

int32_t comp_fxd_coef(comp_prm_t *prm, comp_fxd_coef_t *coef){

    double threshold = pow(10.0, (prm->threshold/20.0));  //in linear
    double ratio = (1.0 - 1.0 / prm->ratio);
    printf("prm->ratio %f, coef->ratio %d\n", prm->ratio, coef->ratio);

    double a_attack = pow(M_e, (-(log(9.0)) / (0.001*prm->tauAttack*prm->samplerate)));
    double a_release = pow(M_e, (-(log(9.0)) / (0.001*prm->tauRelease*prm->samplerate)));
    double e_attack = pow(M_e, (-(log(9.0)) / (0.001*prm->tauEnvAtt*prm->samplerate)));
    double e_release = pow(M_e, (-(log(9.0)) / (0.001*prm->tauEnvRel*prm->samplerate)));

    double makeUpGain = pow(10.0, (prm->makeUpGain/20.0));

    coef->Enable = prm->Enable;



    coef->threshold = dbl_to_fxd(threshold);
    coef->ratio = dbl_to_fxd(ratio);
    coef->a_attack = dbl_to_fxd(a_attack);
    coef->a_release = dbl_to_fxd(a_release);
    coef->e_attack = dbl_to_fxd(e_attack);
    coef->e_release = dbl_to_fxd(e_release);
    coef->makeUpGain = dbl_to_fxd_p(makeUpGain, GAIN_FR);


    printf("coef->e_attack %d\n", coef->e_attack);
    printf("coef->e_release %d\n", coef->e_release);
    printf("coef->a_attack %d\n", coef->a_attack);
    printf("coef->a_release %d\n", coef->a_release);
    printf("coef->makeUpGain  %d\n", coef->makeUpGain);
    printf("prm->samplerate %f\n", prm->samplerate);
    return 0;
}

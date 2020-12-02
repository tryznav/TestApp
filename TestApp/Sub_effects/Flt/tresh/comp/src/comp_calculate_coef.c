#include "comp.h"

#ifndef  M_e
    #define M_e    2.71828182846
#endif

static double dB_to_coeff(double dB){
    return pow(10.0f , (0.05f * dB));
}

int32_t comp_calculate_coef(comp_prm_t *prm, comp_coef_t *coef){
    for(int i = 0; i < CH; i++){
        coef->threshold.ch[i] = (mfloat)pow(10.0, (prm->threshold/20.0));  //in linear
        coef->ratio.ch[i] = (mfloat)prm->ratio;
        // printf("prm->ratio %f, coef->ratio%f\n", prm->ratio, coef->ratio);
        double a_attack   = pow(M_e, (-(log(9.0)) / (0.001*prm->tauAttack*prm->samplerate)));
        double a_release  = pow(M_e, (-(log(9.0)) / (0.001*prm->tauRelease*prm->samplerate)));
        double e_attack   = pow(M_e, (-(log(9.0)) / (0.001*prm->tauEnvAtt*prm->samplerate)));
        double e_release  = pow(M_e, (-(log(9.0)) / (0.001*prm->tauEnvRel*prm->samplerate)));

        coef->a_attack.ch[i]    = (mfloat)a_attack;
        coef->a_release.ch[i]   = (mfloat)a_release;
        coef->a_attack_m.ch[i]  = (mfloat)(1.0 - a_attack);
        coef->a_release_m.ch[i] = (mfloat)(1.0 - a_release);
    
        coef->e_attack.ch[i]    = (mfloat)e_attack;
        coef->e_release.ch[i]   = (mfloat)e_release;
        coef->e_attack_m.ch[i]  = (mfloat)(1.0 - e_attack);
        coef->e_release_m.ch[i] = (mfloat)(1.0 - e_release);

        coef->makeUpGain.ch[i]  = (mfloat)pow(10.0, (prm->makeUpGain/20.0));
    }
    coef->Enable = prm->Enable;
    for(int i = 0; i < CH; i++){
        printf("coef->a_attack.ch[i]    %f\n", coef->a_attack.ch[i]    );
        printf("coef->a_release.ch[i]   %f\n", coef->a_release.ch[i]   );
        printf("coef->a_attack_m.ch[i]  %f\n", coef->a_attack_m.ch[i]  );
        printf("coef->a_release_m.ch[i] %f\n", coef->a_release_m.ch[i] );
        printf("coef->e_attack.ch[i]    %f\n", coef->e_attack.ch[i]    );
        printf("coef->e_release.ch[i]   %f\n", coef->e_release.ch[i]   );
        printf("coef->e_attack_m.ch[i]  %f\n", coef->e_attack_m.ch[i]  );
        printf("coef->e_release_m.ch[i] %f\n", coef->e_release_m.ch[i] );
        printf("coef->makeUpGain.ch[i]  %f\n", coef->makeUpGain.ch[i]  );
    }

    return 0;
}

#include "biquad.h"

#include "params_id.h" 

void biquad_calculate_coef(biquad_prm_t *prm,  biquad_coef_t *coef){
    double  A, sn, cs, alpha, beta, omega;
    double b0, b1, b2, a0, a1, a2;

    A = pow(10, prm->gain / 40);
    omega = 2 * M_PI * prm->freq / prm->sample_rate;
    sn = sin(omega);
    cs = cos(omega);
    alpha = sn / (2 * prm->Q);
    beta  = sqrt(A + A);
    coef->Enable = prm->Enable;

    // printf("prm->freq %f\n",   prm->freq);
    // printf("prm->gain %f\n",   prm->gain);
    // printf("prm->Q %f\n",      prm->Q);
    // printf("prm->sample_rate %f\n", prm->sample_rate);
    switch (prm->type)
    {
    case LPF:
        b0 = (1.0 - cs) / 2.0;
        b1 =  1.0 - cs;
        b2 = (1.0 - cs) / 2.0;
        a0 =  1.0 + alpha;
        a1 = -2.0 * cs;
        a2 =  1.0 - alpha;
        break;
    case HPF:
        b0 =  (1.0 + cs) /2.0;
        b1 = -(1.0 + cs);
        b2 =  (1.0 + cs) /2.0;
        a0 =  1.0 + alpha;
        a1 = -2.0 * cs;
        a2 =  1.0 - alpha;
        break;
    case PQE:
        b0 =  1.0 + (alpha * A);
        b1 = -2.0 * cs;
        b2 =  1.0 - (alpha * A);
        a0 =  1.0 + (alpha / A);
        a1 = -2.0 * cs;
        a2 =  1.0 - (alpha / A);
        break;
    case LSH:
        b0 = A * ((A + 1) - (A - 1) * cs + beta * sn);
        b1 = A * ((A - 1) - (A + 1) * cs) * 2;
        b2 = A * ((A + 1) - (A - 1) * cs - beta * sn); 
        a0 =  (A + 1) + (A - 1) * cs + beta * sn;
        a1 = ((A - 1) + (A + 1) * cs) * (-2);
        a2 =  (A + 1) + (A - 1) * cs - beta * sn;
        break;
    case HSH:
        b0 = A * ((A + 1) + (A - 1) * cs + beta * sn);
        b1 = A * ((A - 1) + (A + 1) * cs) * (-2);
        b2 = A * ((A + 1) + (A - 1) * cs - beta * sn);
        a0 =  (A + 1) - (A - 1) * cs + beta * sn;
        a1 = ((A - 1) - (A + 1) * cs) * 2;
        a2 =  (A + 1) - (A - 1) * cs - beta * sn;
        break;
    case OFF:
        coef->Enable = 0;
        b0 = 1;
        b1 = 0;
        b2 = 0;
        a0 = 0;
        a1 = 0;
        a2 = 0;
        break;
    }

    // a0[i] /= a0[i];
    b0 /= a0;
    b1 /= a0;
    b2 /= a0;
    a1 /= a0;
    a2 /= a0;

    for (size_t i = 0; i < CH; i++) {
        coef->a1.ch[i] = (mfloat)(-a1);
        coef->a2.ch[i] = (mfloat)(-a2);
        coef->b0.ch[i] = (mfloat)(b0);
        coef->b1.ch[i] = (mfloat)(b1);
        coef->b2.ch[i] = (mfloat)(b2);
    }
}

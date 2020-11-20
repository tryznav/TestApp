#include "eq_flt.h"

#include "params_id.h" 

int32_t eq_flt_coef(eq_prm_t *prm,  eq_flt_coef_t *coef){
    double  A[10], sn[10], cs[10], alpha[10], beta[10], omega[10];
    double b0[10], b1[10], b2[10], a0[10], a1[10], a2[10];


    for (size_t i = 0; i < 10; i++) 
    {
        A[i] = pow(10, prm->gain[i] / 40);
        omega[i] = 2 * M_PI * prm->freq[i] / prm->sample_rate;
        printf("freq [%d] %f, sample =%f\n",i, prm->freq[i], prm->sample_rate );
        sn[i] = sin(omega[i]);
        cs[i] = cos(omega[i]);
        alpha[i] = sn[i] / (2 * prm->Q[i]);
        beta[i]  = sqrt(A[i] + A[i]);
        coef->Enable[i] = prm->Enable;
        switch (prm->type[i])
        {
        case LPF:
            b0[i] = (1.0 - cs[i]) / 2.0;
            b1[i] =  1.0 - cs[i];
            b2[i] = (1.0 - cs[i]) / 2.0;
            a0[i] =  1.0 + alpha[i];
            a1[i] = -2.0 * cs[i];
            a2[i] =  1.0 - alpha[i];
            break;
        case HPF:
            b0[i] =  (1.0 + cs[i]) /2.0;
            b1[i] = -(1.0 + cs[i]);
            b2[i] =  (1.0 + cs[i]) /2.0;
            a0[i] =  1.0 + alpha[i];
            a1[i] = -2.0 * cs[i];
            a2[i] =  1.0 - alpha[i];
            break;
        case PQE:
            b0[i] =  1.0 + (alpha[i] * A[i]);
            b1[i] = -2.0 * cs[i];
            b2[i] =  1.0 - (alpha[i] * A[i]);
            a0[i] =  1.0 + (alpha[i] / A[i]);
            a1[i] = -2.0 * cs[i];
            a2[i] =  1.0 - (alpha[i] / A[i]);
            break;
        case LSH:
            b0[i] = A[i] * ((A[i] + 1) - (A[i] - 1) * cs[i] + beta[i] * sn[i]);
    	    b1[i] = A[i] * ((A[i] - 1) - (A[i] + 1) * cs[i]) * 2;
    	    b2[i] = A[i] * ((A[i] + 1) - (A[i] - 1) * cs[i] - beta[i] * sn[i]); 
    	    a0[i] =  (A[i] + 1) + (A[i] - 1) * cs[i] + beta[i] * sn[i];
    	    a1[i] = ((A[i] - 1) + (A[i] + 1) * cs[i]) * (-2);
    	    a2[i] =  (A[i] + 1) + (A[i] - 1) * cs[i] - beta[i] * sn[i];
            break;
        case HSH:
            b0[i] = A[i] * ((A[i] + 1) + (A[i] - 1) * cs[i] + beta[i] * sn[i]);
            b1[i] = A[i] * ((A[i] - 1) + (A[i] + 1) * cs[i]) * (-2);
            b2[i] = A[i] * ((A[i] + 1) + (A[i] - 1) * cs[i] - beta[i] * sn[i]);
            a0[i] =  (A[i] + 1) - (A[i] - 1) * cs[i] + beta[i] * sn[i];
            a1[i] = ((A[i] - 1) - (A[i] + 1) * cs[i]) * 2;
            a2[i] =  (A[i] + 1) - (A[i] - 1) * cs[i] - beta[i] * sn[i];
            break;
        case OFF:
            coef->Enable[i] = 0;
            coef->a1[i] = 0;
            coef->a2[i] = 0;
            coef->b0[i] = 0;
            coef->b1[i] = 0;
            coef->b2[i] = 0;
            break;
        }

        // a0[i] /= a0[i];
        b0[i] /= a0[i];
        b1[i] /= a0[i];
        b2[i] /= a0[i];
        a1[i] /= a0[i];
        a2[i] /= a0[i];



        coef->a1[i] = (coef_type)(a1[i]);
        coef->a2[i] = (coef_type)(a2[i]);
        coef->b0[i] = (coef_type)(b0[i]);
        coef->b1[i] = (coef_type)(b1[i]);
        coef->b2[i] = (coef_type)(b2[i]);

    }
    return 0;
}

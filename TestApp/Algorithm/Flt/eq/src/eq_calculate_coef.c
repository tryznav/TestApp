#include "eq.h"

#if CH == 2
void eq_calculate_coef(eq_prm_t *prm,  eq_coef_t *coef){
    for (int32_t i = 0; i < EQ_BAND; i++) 
    {
        biquad_calculate_coef(&prm->biquad_prm[i], &coef->biquad_coef[i]); 
    }
}
#elif CH == 4

#elif CH == 8

void eq_calculate_coef(eq_prm_t *prm,  eq_coef_t *coef){
    for (int32_t i = 0; i < EQ_BAND; i++) 
    {
        for(int i = 0, y = 0, x = 0; i < EQ_BAND; i++){
            biquad_prm_t *bprm = &prm->biquad_prm[i];
            double  A, sn, cs, alpha, beta, omega;
            double b0, b1, b2, a0, a1, a2;

            A = pow(10, bprm->gain / 40);
            omega = 2 * M_PI * bprm->freq / bprm->sample_rate;
            sn = sin(omega);
            cs = cos(omega);
            alpha = sn / (2 * bprm->Q);
            beta  = sqrt(A + A);

            switch (bprm->type)
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

            coef->a1[x].ch[y] = (mfloat)(-a1);
            coef->a2[x].ch[y] = (mfloat)(-a2);
            coef->b0[x].ch[y] = (mfloat)(b0);
            coef->b1[x].ch[y] = (mfloat)(b1);
            coef->b2[x].ch[y] = (mfloat)(b2);
            
            coef->a1[x].ch[y+1] = (mfloat)(-a1);
            coef->a2[x].ch[y+1] = (mfloat)(-a2);
            coef->b0[x].ch[y+1] = (mfloat)(b0);
            coef->b1[x].ch[y+1] = (mfloat)(b1);
            coef->b2[x].ch[y+1] = (mfloat)(b2);

            printf("coef->a1[x].ch[y] %f\n", coef->a1[x].ch[y]);
            printf("coef->a2[x].ch[y] %f\n", coef->a2[x].ch[y]);
            printf("coef->b0[x].ch[y] %f\n", coef->b0[x].ch[y]);
            printf("coef->b1[x].ch[y] %f\n", coef->b1[x].ch[y]);
            printf("coef->b2[x].ch[y] %f\n", coef->b2[x].ch[y]);

            y+=2;
            if(y > 7){
                y = 0;
                x++;
            }
        }
    }
}
#endif
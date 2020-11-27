#include "cross.h"

static double dB_to_coeff(double dB){
    return pow(10.0f , (0.05f * dB));
}

void cross_calculate_coef(cross_prm_t *prm, cross4b_coef_t *coef){
    for(int i = 0; i < 3; i++){
        double Q = 0.70710678118;
        double fb = (prm->freq[i] / Q);

        double c = (tan(M_PI *  fb / prm->sample_rate) - 1) / (tan(M_PI *  fb / prm->sample_rate) + 1);
        double d = -cos(2 * M_PI  * prm->freq[i] / prm->sample_rate);
        double dc = d * (1.0 - c);

        double c1 = (tan(M_PI * prm->freq[i] / prm->sample_rate) - 1) / (tan(M_PI *  prm->freq[i] / prm->sample_rate) + 1);
    printf("prm->freq %f\n",   prm->freq[i]);
    printf("prm->sample_rate %f\n",  prm->sample_rate);
    printf("fb %f\n",      fb);
#if AVX
    #if CH == 8
        switch (i)
        {
        case 0:
            for(int x = 0; x < 2; x++){
                coef->f0_f2.k1.ch[x] = (mfloat)dc;
                coef->f0_f2.k2.ch[x] = (mfloat)-c;
                coef->f0_f2.k.ch[x] = (mfloat)c1;
                
                // printf("coef->f[i].k1.ch[] = (mfloat)dc %f\n", coef->f[i].k1.ch[x]);
                // printf("coef->f[i].k2.ch[i] = (mfloat)-c %f\n" ,coef->f[i].k2.ch[x]);
                // printf("coef->f[i].k.ch[i] = (mfloat)c1 %f\n", coef->f[i].k.ch[x]);
            }
            for(int x = 2; x < 4; x++){
                coef->f0_f2.k1_c.ch[x] = (mfloat)dc;
                coef->f0_f2.k2_c.ch[x] = (mfloat)-c;  
                // printf("coef->f[i].k1.ch[] = (mfloat)dc %f\n", coef->f[i].k1.ch[x]);
                // printf("coef->f[i].k2.ch[i] = (mfloat)-c %f\n" ,coef->f[i].k2.ch[x]);
                // printf("coef->f[i].k.ch[i] = (mfloat)c1 %f\n", coef->f[i].k.ch[x]);
            }
            break;
        case 1:
            for(int x = 0; x < CH; x++){
                coef->f1.k1.ch[x] = (mfloat)dc;
                coef->f1.k2.ch[x] = (mfloat)-c;
                coef->f1.k.ch[x] = (mfloat)c1;
                coef->f1.half.ch[x] = (mfloat)0.5;
                coef->f0_f2.half.ch[x] = (mfloat)0.5;
                // printf("coef->f[i].k1.ch[] = (mfloat)dc %f\n", coef->f[i].k1.ch[x]);
                // printf("coef->f[i].k2.ch[i] = (mfloat)-c %f\n" ,coef->f[i].k2.ch[x]);
                // printf("coef->f[i].k.ch[i] = (mfloat)c1 %f\n", coef->f[i].k.ch[x]);
            }
            break;
        case 2:
            /* code */
            break;
        
        default:
            break;
        }
            for(int x = 2; x < 4; x++){
                coef->f0_f2.k1.ch[x] = (mfloat)dc;
                coef->f0_f2.k2.ch[x] = (mfloat)-c;
                coef->f0_f2.k.ch[x] = (mfloat)c1;
                
                // printf("coef->f[i].k1.ch[] = (mfloat)dc %f\n", coef->f[i].k1.ch[x]);
                // printf("coef->f[i].k2.ch[i] = (mfloat)-c %f\n" ,coef->f[i].k2.ch[x]);
                // printf("coef->f[i].k.ch[i] = (mfloat)c1 %f\n", coef->f[i].k.ch[x]);
            }
            for(int x = 0; x < 2; x++){
                coef->f0_f2.k1_c.ch[x] = (mfloat)dc;
                coef->f0_f2.k2_c.ch[x] = (mfloat)-c;  
                // printf("coef->f[i].k1.ch[] = (mfloat)dc %f\n", coef->f[i].k1.ch[x]);
                // printf("coef->f[i].k2.ch[i] = (mfloat)-c %f\n" ,coef->f[i].k2.ch[x]);
                // printf("coef->f[i].k.ch[i] = (mfloat)c1 %f\n", coef->f[i].k.ch[x]);
            }
    #endif
#elif SSE
    typedef struct band4_s{
        vfloat;
    }band2_t;
    typedef struct band4_s{
        band2_t low;
        band2_t high;
    }band4_t;
#else
    #if CH == 2
        for(int x = 0; x < CH; x++){
            coef->f[i].k1.ch[x] = (mfloat)dc;
            coef->f[i].k2.ch[x] = (mfloat)-c;
            coef->f[i].k.ch[x] = (mfloat)c1;
            printf("coef->f[i].k1.ch[] = (mfloat)dc %f\n", coef->f[i].k1.ch[x]);
            printf("coef->f[i].k2.ch[i] = (mfloat)-c %f\n" ,coef->f[i].k2.ch[x]);
            printf("coef->f[i].k.ch[i] = (mfloat)c1 %f\n", coef->f[i].k.ch[x]);
        }
    #elif CH == 4
        typedef struct band2_s{
            vfloat  band1; //low
            vfloat  band2; //high
        }band2_t;

        typedef struct band4_s{
        band2_t low;
        band2_t high;
        }band4_t;

    #elif CH == 8
        switch (i)
        {
        case 0:
            for(int x = 0; x < 2; x++){
                coef->f0_f2.k1.ch[x] = (mfloat)dc;
                coef->f0_f2.k2.ch[x] = (mfloat)-c;
                coef->f0_f2.k.ch[x] = (mfloat)c1;
                
                // printf("coef->f[i].k1.ch[] = (mfloat)dc %f\n", coef->f[i].k1.ch[x]);
                // printf("coef->f[i].k2.ch[i] = (mfloat)-c %f\n" ,coef->f[i].k2.ch[x]);
                // printf("coef->f[i].k.ch[i] = (mfloat)c1 %f\n", coef->f[i].k.ch[x]);
            }
            for(int x = 2; x < 4; x++){
                coef->f0_f2.k1_c.ch[x] = (mfloat)dc;
                coef->f0_f2.k2_c.ch[x] = (mfloat)-c;  
                // printf("coef->f[i].k1.ch[] = (mfloat)dc %f\n", coef->f[i].k1.ch[x]);
                // printf("coef->f[i].k2.ch[i] = (mfloat)-c %f\n" ,coef->f[i].k2.ch[x]);
                // printf("coef->f[i].k.ch[i] = (mfloat)c1 %f\n", coef->f[i].k.ch[x]);
            }
            break;
        case 1:
            for(int x = 0; x < CH; x++){
                coef->f1.k1.ch[x] = (mfloat)dc;
                coef->f1.k2.ch[x] = (mfloat)-c;
                coef->f1.k.ch[x] = (mfloat)c1;
                coef->f1.half.ch[x] = (mfloat)0.5;
                coef->f0_f2.half.ch[x] = (mfloat)0.5;
                // printf("coef->f[i].k1.ch[] = (mfloat)dc %f\n", coef->f[i].k1.ch[x]);
                // printf("coef->f[i].k2.ch[i] = (mfloat)-c %f\n" ,coef->f[i].k2.ch[x]);
                // printf("coef->f[i].k.ch[i] = (mfloat)c1 %f\n", coef->f[i].k.ch[x]);
            }
            break;
        case 2:
            /* code */
            break;
        
        default:
            break;
        }
            for(int x = 2; x < 4; x++){
                coef->f0_f2.k1.ch[x] = (mfloat)dc;
                coef->f0_f2.k2.ch[x] = (mfloat)-c;
                coef->f0_f2.k.ch[x] = (mfloat)c1;
                
                // printf("coef->f[i].k1.ch[] = (mfloat)dc %f\n", coef->f[i].k1.ch[x]);
                // printf("coef->f[i].k2.ch[i] = (mfloat)-c %f\n" ,coef->f[i].k2.ch[x]);
                // printf("coef->f[i].k.ch[i] = (mfloat)c1 %f\n", coef->f[i].k.ch[x]);
            }
            for(int x = 0; x < 2; x++){
                coef->f0_f2.k1_c.ch[x] = (mfloat)dc;
                coef->f0_f2.k2_c.ch[x] = (mfloat)-c;  
                // printf("coef->f[i].k1.ch[] = (mfloat)dc %f\n", coef->f[i].k1.ch[x]);
                // printf("coef->f[i].k2.ch[i] = (mfloat)-c %f\n" ,coef->f[i].k2.ch[x]);
                // printf("coef->f[i].k.ch[i] = (mfloat)c1 %f\n", coef->f[i].k.ch[x]);
            }
    #endif
#endif

    }
}

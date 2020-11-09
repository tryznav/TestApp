#include "cross_flt.h"

static double dB_to_coeff(double dB){
    return pow(10.0f , (0.05f * dB));
}

int32_t cross_flt_coef(cross_prm_t *prm, cross_coef_t *coef){
    for(int freq = 0; freq < NUM_OF_FREQ; freq++){
        apf_flt_1st_coef(&(coef->apf_1st[freq]), prm->freq[freq], prm->sample_rate, prm->apf_form1);
        apf_flt_2nd_coef(&(coef->apf_2nd[freq]), prm->freq[freq], prm->sample_rate, prm->apf_form2);
    }

    switch (prm->apf_form1)
    {
    case 1:
        coef->apf_1st_f = &apf_flt_1st_DirectI;
        break;
    case 2:
        coef->apf_1st_f = &apf_flt_1st_DirectII;
        break;
    default:
        fprintf(stderr, RED "Error:\t"RESET BOLDWHITE"Wrong APF form, app has 1-2. Rejected\n"RESET);
        break;
    }
    switch (prm->apf_form2)
    {
    case 1:
        coef->apf_2nd_f = &apf_flt_2nd_DirectI;
        break;
    case 2:
        coef->apf_2nd_f = &apf_flt_2nd_DirectI_t;
        break;
    case 3:
        coef->apf_2nd_f = &apf_flt_2nd_DirectII;
        break;
    case 4:
        coef->apf_2nd_f = &apf_flt_2nd_DirectII_t;
        break;
    case 5:    //Nested form coef
        coef->apf_2nd_f = &apf_2nd_Lattice;
        break;
    default:
        fprintf(stderr, RED "Error:\t"RESET BOLDWHITE"Wrong APF form, app has 1-5. Rejected\n"RESET);
        break;
    }
    coef->apf_1st_f_dbl = apf_dbl_1st_DirectII;
    coef->apf_2nd_f_dbl = apf_dbl_2nd_DirectI;
    return 0;
}

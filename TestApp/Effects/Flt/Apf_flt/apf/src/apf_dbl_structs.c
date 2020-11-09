#include "apf_flt.h"

// First order All-Pass Filter

double apf_dbl_1st_DirectII(double x, apf_states_t *st, apf_coef_t *coef){
    double xh = x;
    xh -= coef->k_dbl[0] * st->xh_dbl[0];

    double y = st->xh_dbl[0] + (xh * coef->k_dbl[0]);

    st->xh_dbl[0] = xh;

    return y;
}

//+++++++++++Second order All-Pass Filter+++++++++++

double apf_dbl_2nd_DirectI(double x, apf_states_t *st, apf_coef_t *coef){
    double xh = 0;

    xh += (coef->k_dbl[1] * x);
    xh += (coef->k_dbl[0] * st->xh_dbl[0]);

    xh += st->xh_dbl[1];


    xh -= (coef->k_dbl[0] * st->y_dbl[0]);
    xh -= (coef->k_dbl[1] * st->y_dbl[1]);  


    st->xh_dbl[1] = st->xh_dbl[0];
    st->xh_dbl[0] = x;

    st->y_dbl[1] = st->y_dbl[0];
    st->y_dbl[0] =  xh;

    return xh;
}

//Nested Forms

double apf_dbl_2nd_lattice(double x, apf_states_t *st, apf_coef_t *coef){
    double xh = x;

    xh -= coef->k_dbl[1] *  st->xh_dbl[1];
    double y = st->xh_dbl[1] + (coef->k_dbl[1] * xh);

    st->xh_dbl[1] = apf_dbl_1st_DirectII(xh, st, coef);

    return y;
}
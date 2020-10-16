
#include "test_app.h"

effect_task_t *app_parse_iir_opt(char * opt){
    char *sapmle_rate_ch = NULL;
    char *length_ms_ch = NULL;
    char *iir_type = NULL;
    char *ifn = NULL;
    effect_task_t * effect;

    effect = malloc (sizeof(effect_task_t));
    if(!effect){
        fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
        exit(EXIT_FAILURE);
    }

    effect->effect_type = EFFECT_ID_IIR;

    iir_type = strtok(NULL, ":");
    if(!iir_type){
    fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
    exit(EXIT_FAILURE);
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if(strstr( iir_type, "LPF")){
        effect->effect_subtype = LPF;
        ifn = strtok(NULL, ",");
        if(!ifn){
            fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
            exit(EXIT_FAILURE);
        }
        return effect;
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if(strstr( Sig_type, "step")){
    generator->signal_id = TSIG_STEP;
    return generator;
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if(strstr( Sig_type, "square")){
    generator->signal_id = TSIG_SQUARE;
    ifn = strtok(NULL, "");

    if(!ifn){
        fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
        exit(EXIT_FAILURE);
    }
    generator->period_ms = (uint32_t)atol(ifn);
    return generator;
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if(strstr( Sig_type, "wnoise")){
    generator->signal_id = TSIG_WNOISE;
    ifn = strtok(NULL, ",");

    if(!ifn){
        fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
        exit(EXIT_FAILURE);
    }
    generator->amp_dB.whole_file = (float)atof(ifn);
    return generator;
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if(strstr( Sig_type, "sine")){
    generator->signal_id = TSIG_SINE;
    ifn = strtok(NULL, ",");

    if(!ifn){
        fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
        exit(EXIT_FAILURE);
    }
    generator->freq_Hz.whole_file = (float)atof(ifn);
    ifn = strtok(NULL, ",");

    if(!ifn){
        fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
        exit(EXIT_FAILURE);
    }
    generator->amp_dB.whole_file = (float)atof(ifn);
    return generator;
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if(strstr( Sig_type, "chirp_linear")){
    generator->signal_id = TSIG_CHIRP_LINEAR;
    ifn = strtok(NULL, ",");
    if(!ifn){
        fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
        exit(EXIT_FAILURE);
    }
    generator->freq_Hz.sweep.start = (float)atof(ifn);
    ifn = strtok(NULL, ",");
    if(!ifn){
        fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
        exit(EXIT_FAILURE);
    }
    generator->freq_Hz.sweep.end  = (float)atof(ifn);
    ifn = strtok(NULL, ",");
    if(!ifn){
        fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
        exit(EXIT_FAILURE);
    }
    generator->amp_dB.whole_file = (float)atof(ifn);
    return generator;
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if(strstr( Sig_type, "chirp_logarithmic")){
    generator->signal_id = TSIG_CHIRP_LOGARITM;
    ifn = strtok(NULL, ",");
    if(!ifn){
        fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
        exit(EXIT_FAILURE);
    }
    generator->freq_Hz.sweep.start = (float)atof(ifn);
    ifn = strtok(NULL, ",");
    if(!ifn){
        fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
        exit(EXIT_FAILURE);
    }
    generator->freq_Hz.sweep.end  = (float)atof(ifn);
    ifn = strtok(NULL, ",");
    if(!ifn){
        fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
        exit(EXIT_FAILURE);
    }
    generator->amp_dB.whole_file = (float)atof(ifn);
    return generator;
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if(strstr( Sig_type, "lsweep")){
    generator->signal_id = TSIG_LEVEL_SWEEP;

    ifn = strtok(NULL, ",");
    if(!ifn){
        fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
        exit(EXIT_FAILURE);
    }
    generator->freq_Hz.whole_file = (float)atof(ifn);

    ifn = strtok(NULL, ",");
    if(!ifn){
        fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
        exit(EXIT_FAILURE);
    }
    generator->amp_dB.sweep.start = (float)atof(ifn);

    ifn = strtok(NULL, ",");
    if(!ifn){
        fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
        exit(EXIT_FAILURE);
    }
    generator->amp_dB.sweep.end  = (float)atof(ifn);
    return generator;
    }
    fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
    return NULL;
    }

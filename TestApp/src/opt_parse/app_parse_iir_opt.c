
#include "test_app.h"

effect_task_t *app_parse_iir_opt(char * opt){
    char *sapmle_rate_ch = NULL;
    char *length_ms_ch = NULL;
    char *iir_type = NULL;
    char *ifn = NULL;
    effect_task_t * effect;
                    

    effect = malloc (sizeof(effect_task_t));
    if(!effect){
        fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --iir. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
        exit(EXIT_FAILURE);
    }

    effect->effect_type = EFFECT_ID_IIR;

    iir_type = strtok(opt, ":");
    if(!iir_type){
        fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --iir. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
        exit(EXIT_FAILURE);
    }
    // printf("here2");
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if(strstr( iir_type, "LPF")){
        effect->effect_subtype = LPF;

        ifn = strtok(NULL, ",");
        if(!ifn){
            fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --iir. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
            exit(EXIT_FAILURE);
        }
        effect->cutoff_freq_Hz.whole_file = (float)atof(ifn);

        return effect;
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if(strstr(iir_type, "HPF")){
        effect->effect_subtype = HPF;
        ifn = strtok(NULL, ",");
        if(!ifn){
            fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --iir. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
            exit(EXIT_FAILURE);
        }
        effect->cutoff_freq_Hz.whole_file = (float)atof(ifn);
        return effect;
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if(strstr(iir_type, "BPF")){
        effect->effect_subtype = BPF;
        ifn = strtok(NULL, ",");
        if(!ifn){
            fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --iir. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
            exit(EXIT_FAILURE);
        }
        effect->cutoff_freq_Hz.sweep.start = (float)atof(ifn);

        ifn = strtok(NULL, ",");
        if(!ifn){
            fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --iir. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
            exit(EXIT_FAILURE);
        }
        effect->cutoff_freq_Hz.sweep.end = (float)atof(ifn);
        return effect;
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if(strstr(iir_type, "BSF")){
        effect->effect_subtype = BSF;
        ifn = strtok(NULL, ",");
        if(!ifn){
            fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --iir. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
            exit(EXIT_FAILURE);
        }
        effect->cutoff_freq_Hz.sweep.start = (float)atof(ifn);

        ifn = strtok(NULL, ",");
        if(!ifn){
            fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --iir. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
            exit(EXIT_FAILURE);
        }
        effect->cutoff_freq_Hz.sweep.end = (float)atof(ifn);
        return effect;
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if(strstr(iir_type, "PQE")){
        effect->effect_subtype = PQE;
        ifn = strtok(NULL, ",");
        if(!ifn){
            fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --iir. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
            exit(EXIT_FAILURE);
        }
        effect->cutoff_freq_Hz.sweep.start = (float)atof(ifn);

        ifn = strtok(NULL, ",");
        if(!ifn){
            fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --iir. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
            exit(EXIT_FAILURE);
        }
        effect->amp_dB.whole_file = (float)atof(ifn);
        return effect;
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if(strstr(iir_type, "LSH")){
        effect->effect_subtype = LSH;
        ifn = strtok(NULL, ",");
        if(!ifn){
            fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --iir. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
            exit(EXIT_FAILURE);
        }
        effect->cutoff_freq_Hz.sweep.start = (float)atof(ifn);
        ifn = strtok(NULL, ",");
        if(!ifn){
            fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --iir. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
            exit(EXIT_FAILURE);
        }
        effect->amp_dB.whole_file = (float)atof(ifn);
        return effect;
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if(strstr(iir_type, "HSH")){
        effect->effect_subtype = HSH;
        ifn = strtok(NULL, ",");
        if(!ifn){
            fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --iir. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
            exit(EXIT_FAILURE);
        }
        effect->cutoff_freq_Hz.sweep.start = (float)atof(ifn);

        ifn = strtok(NULL, ",");
        if(!ifn){
            fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --iir. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
            exit(EXIT_FAILURE);
        }
        effect->amp_dB.whole_file = (float)atof(ifn);
        return effect;
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --iir. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
    return NULL;
    }

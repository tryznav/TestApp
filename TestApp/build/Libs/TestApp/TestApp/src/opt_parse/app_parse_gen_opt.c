#include "test_app.h"

gen_prm_t *app_parse_gen_opt(char * opt){
  char *sapmle_rate_ch = NULL;
  char *length_ms_ch = NULL;
  char *Sig_type = NULL;
  char *ifn = NULL;
  gen_prm_t * generator;

  generator = malloc (sizeof(gen_prm_t));
  if(!generator){
    fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
    exit(EXIT_FAILURE);
  }

  sapmle_rate_ch = strtok(opt, ",");
  if(!sapmle_rate_ch){
    fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
    exit(EXIT_FAILURE);
  }
  generator->sample_rate = (uint32_t)atol(sapmle_rate_ch);


  length_ms_ch = strtok(NULL, ",");
  if(!length_ms_ch){
    fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
    exit(EXIT_FAILURE);
  }
  generator->length_ms = (uint32_t)atol(length_ms_ch);
  // printf("length_ms_c = %d\n", task->length_ms);

  Sig_type = strtok(NULL, ":");
  if(!Sig_type){
    fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
    exit(EXIT_FAILURE);
  }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  if(strstr( Sig_type, "delta")){
   generator->signal_id = TSIG_DELTA;
   printf("DELTA\n");
   return generator;
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

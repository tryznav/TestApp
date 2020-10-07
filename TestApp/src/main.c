#include "test_app.h"

static int parse_generator_opt(char * opt, app_func_t *task );
static void print_help(void);

main (int argc, char **argv)
{
  int c;
  app_func_t *app_task = (app_func_t *)malloc(sizeof(app_func_t));
  memset(app_task,0, sizeof(app_func_t));

  while (1){
    static struct option long_options[] =
      {
        {"help",      no_argument,       0, 'h'},
        {"print-hdr", no_argument,       0, 'p'},
        {"gain",      required_argument, 0, 'g'},
        {"in",        required_argument, 0, 'i'},
        {"out",       required_argument, 0, 'o'},
        {"lpf",       required_argument, 0, 'l'},
        {"PMC",       no_argument,       0, 'q'},
        {"IEEE_754",  no_argument,       0, 'f'},
        {"generator", required_argument, 0, 's'},
        {0, 0, 0, 0}
      };
    /* getopt_long stores the option index here. */
    int option_index = 0;

    c = getopt_long (argc, argv, "hpo:g:i:l:qf",
                      long_options, &option_index);

    /* Detect the end of the options. */
    if (c == -1)
      break;

    switch (c)
      {
      case 0:
        /* If this option set a flag, do nothing else now. */
        if (long_options[option_index].flag != 0)
          break;
        printf ("option %s", long_options[option_index].name);
        if (optarg)
          printf (" with arg %s", optarg);
        printf ("\n");
        break;
      case 'p':
        app_task->print_hdr = true;
        break;
      case 'h':
        print_help();
        exit(EXIT_SUCCESS);
        break;
      case 'g':
        printf ("option -o with value `%s'\n", optarg);
        app_task->gain = true;
        app_task->gain_dB = (float)atof(optarg);                             //????????????????????????????
        break;
      case 'i':
        app_task->input = true;
        app_task->input_f_path = (char *)malloc(strlen(optarg) + 1);
        memcpy(app_task->input_f_path, optarg, (strlen(optarg) + 1));
        break;
      case 'o':
        app_task->ouput = true;
        app_task->output_f_path = malloc(strlen(optarg) + 1);
        memcpy(app_task->output_f_path, optarg, (strlen(optarg) + 1));
        break;
      case 's':
        parse_generator_opt(optarg, app_task);
        break;
      case 'l':
        // app_task->audioFormat = true;
        
        // memcpy(app_task->output_f_path, optarg, (strlen(optarg) + 1));
        break;
      case 'f':
          app_task->audioFormat = true;
          app_task->audioFormatType = IEEE_754;
        break;
      case 'q':
          app_task->audioFormat = true;
          app_task->audioFormatType = PMC;
        break;
      case '?':
        /* getopt_long already printed an error message. */
        break;

      default:
        fprintf(stderr, RED "Error:\t"RESET BOLDWHITE"Unknown option. Rejected\n"RESET);
      }
  }

  /* Print any remaining command line arguments (not options). */
  if (optind < argc){
      fprintf(stderr, RED "Error: "RESET BOLDWHITE"non-option ARGV-element: "RESET);
      while (optind < argc)
          printf ("%s ", argv[optind++]);
      fprintf(stderr,RED "\nRejected.\n"RESET);
      exit (EXIT_FAILURE);
  }
  app_handler(app_task);
  if(app_task->input){
    free(app_task->input_f_path);
  }
  if(app_task->ouput){
    free(app_task->output_f_path);
  }

  exit (EXIT_SUCCESS);
}

static int parse_generator_opt(char * opt, app_func_t *task ){
  char *sapmle_rate_ch = NULL;
  char *length_ms_ch = NULL;
  char *Sig_type = NULL;
  char *ifn = NULL;

  task->generator =  true;

  sapmle_rate_ch = strtok(opt, ",");
  if(!sapmle_rate_ch){
    fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
    exit(EXIT_FAILURE);
  }
  task->sample_rate = (uint32_t)atol(sapmle_rate_ch);


  length_ms_ch = strtok(NULL, ",");
  if(!length_ms_ch){
    fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
    exit(EXIT_FAILURE);
  }
  task->length_ms = (uint32_t)atol(length_ms_ch);
  // printf("length_ms_c = %d\n", task->length_ms);

  Sig_type = strtok(NULL, ":");
  if(!Sig_type){
    fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
    exit(EXIT_FAILURE);
  }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  if(strstr( Sig_type, "delta")){
   task->signal_id = TSIG_DELTA;
   return 0;
  }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  if(strstr( Sig_type, "step")){
    task->signal_id = TSIG_STEP;
    return 0;
  }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  if(strstr( Sig_type, "square")){
    task->signal_id = TSIG_SQUARE;
    ifn = strtok(NULL, "");

    if(!ifn){
      fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
      exit(EXIT_FAILURE);
    }
    task->period_ms = (uint32_t)atol(ifn);
    return 0;
  }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  if(strstr( Sig_type, "wnoise")){
    task->signal_id = TSIG_WNOISE;
    ifn = strtok(NULL, ",");

    if(!ifn){
      fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
      exit(EXIT_FAILURE);
    }
    task->amplitude.whole_file = (float)atof(ifn);
    return 0;
  }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  if(strstr( Sig_type, "sine")){
    task->signal_id = TSIG_SINE;
    ifn = strtok(NULL, ",");

    if(!ifn){
      fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
      exit(EXIT_FAILURE);
    }
    task->frequency.whole_file_freq = (uint32_t)atol(ifn);
    ifn = strtok(NULL, ",");

    if(!ifn){
      fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
      exit(EXIT_FAILURE);
    }
    task->amplitude.whole_file = (float)atof(ifn);
    return 0;
  }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  if(strstr( Sig_type, "chirp_linear")){
    task->signal_id = TSIG_CHIRP_LINEAR;
    ifn = strtok(NULL, ",");
    if(!ifn){
      fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
      exit(EXIT_FAILURE);
    }
    task->frequency.start_freq = (uint32_t)atol(ifn);
    ifn = strtok(NULL, ",");
    if(!ifn){
      fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
      exit(EXIT_FAILURE);
    }
    task->frequency.end_freq  = (uint32_t)atol(ifn);
    ifn = strtok(NULL, ",");
    if(!ifn){
      fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
      exit(EXIT_FAILURE);
    }
    task->amplitude.whole_file = (float)atof(ifn);
    return 0;
  }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  if(strstr( Sig_type, "chirp_logarithmic")){
    task->signal_id = TSIG_CHIRP_LOGARITM;
    ifn = strtok(NULL, ",");
    if(!ifn){
      fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
      exit(EXIT_FAILURE);
    }
    task->frequency.start_freq = (uint32_t)atol(ifn);
    ifn = strtok(NULL, ",");
    if(!ifn){
      fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
      exit(EXIT_FAILURE);
    }
    task->frequency.end_freq  = (uint32_t)atol(ifn);
    ifn = strtok(NULL, ",");
    if(!ifn){
      fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
      exit(EXIT_FAILURE);
    }
    task->amplitude.whole_file = (float)atof(ifn);
    return 0;
  }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  if(strstr( Sig_type, "lsweep")){
    task->signal_id = TSIG_LEVEL_SWEEP;
  
    ifn = strtok(NULL, ",");
    if(!ifn){
      fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
      exit(EXIT_FAILURE);
    }
    task->frequency.whole_file_freq = (uint32_t)atol(ifn);
  
    ifn = strtok(NULL, ",");
    if(!ifn){
      fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
      exit(EXIT_FAILURE);
    }
    task->amplitude.start_amp_dB = (float)atof(ifn);
  
    ifn = strtok(NULL, ",");
    if(!ifn){
      fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
      exit(EXIT_FAILURE);
    }
    task->amplitude.end_amp_db = (float)atof(ifn);
    return 0;
  }
  fprintf(stderr, RED "Error: "RESET BOLDWHITE"'%s' non-arg --generator. Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET, opt);
  return -1;
}

static void print_help(void){
  printf("%s",HELP_TEXT_OPT);
  printf("%s",HELP_TEXT_IN);
  printf("%s",HELP_TEXT_OUT);
  printf("%s",HELP_TEXT_P_HDR);
  printf("%s",HELP_TEXT_GAIN);
  printf("%s",HELP_TEXT_GENER_1);
  printf("%s",HELP_TEXT_GENER_2);
  printf("%s",HELP_TEXT_GEN_S_T);
  printf("%s",HELP_TEXT_DELTA);
  printf("%s",HELP_TEXT_STEP);
  printf("%s",HELP_TEXT_SQUARE);
  printf("%s",HELP_TEXT_WNOISE);
  printf("%s",HELP_TEXT_SINE);
  printf("%s",HELP_TEXT_CHIRP_LIN);
  printf("%s",HELP_TEXT_CHIRP_LOG);
  printf("%s",HELP_TEXT_lSWEEP);
}

#include "test_app.h"

static void print_help(void);

main (int argc, char **argv)
{
  int c;
  app_func_t *app_task = (app_func_t *)malloc(sizeof(app_func_t));
  memset(app_task,0, sizeof(app_func_t));
  app_task->audioFormatType = IEEE_754;
  // char *fin;

  while (1){
    static struct option long_options[] =
      {
        {"help",      no_argument,       0, 'h'},
        {"print-hdr", no_argument,       0, 'p'},
        {"gain",      required_argument, 0, 'g'},
        {"in",        required_argument, 0, 'i'},
        {"out",       required_argument, 0, 'o'},
        {"fir_bpf",   no_argument,       0, 'b'},
        {"iir",       required_argument, 0, 'r'},
        {"PCM",       no_argument,       0, 'q'},
        {"IEEE_754",  no_argument,       0, 'f'},
        {"generator", required_argument, 0, 's'},
        {"crossover", no_argument, 0, 'c'},
        {"apf",       no_argument, 0, 'a'},
        {0, 0, 0, 0}
      };
    /* getopt_long stores the option index here. */
    int option_index = 0;

    c = getopt_long (argc, argv, "chpqfo:g:i:b:r:s:",
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
        app_task->effect = malloc(sizeof(effect_task_t));
        app_task->effect->effect_type = EFFECT_ID_GAIN;
        app_task->effect->amp_dB.whole_file = (float)atof(optarg);                           //????????????????????????????
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
        app_task->generator = app_parse_gen_opt(optarg);
        if(!app_task->generator){
          printf("error");
        }
        break;
      case 'b':
        app_task->effect = malloc(sizeof(effect_task_t));
        app_task->effect->effect_type =  EFFECT_ID_FIR;
        break;
      case 'r':
        app_task->effect = app_parse_iir_opt(optarg);
        if(!app_task->effect){
          printf("error");
        }
        break;
      case 'c':
        app_task->audioFormatType = IEEE_754;
        app_task->effect = malloc(sizeof(effect_task_t));
        app_task->effect->effect_type = EFFECT_ID_CROSSOVER;
        // printf(" EFFECT_ID_FIR");
        app_task->effect->cutoff_freq_Hz.sweep.start = 0;// (float)atof(optarg);
        // fin = strchr(optarg, '_');
        // fin = fin + 1;
        app_task->effect->cutoff_freq_Hz.sweep.end = 0; //(float)atof(fin);
        // printf("prm.cutoff_freq.sweep.start %f\n", app_task->effect->prm.cutoff_freq.sweep.start );
        // printf("app_task->effect->prm.cutoff_freq.sweep.end %f\n", app_task->effect->prm.cutoff_freq.sweep.end);
        break;
      case 'a':
        app_task->audioFormatType = IEEE_754;
        app_task->effect = malloc(sizeof(effect_task_t));
        app_task->effect->effect_type = EFFECT_ID_APF;
        // printf(" EFFECT_ID_FIR");
        app_task->effect->cutoff_freq_Hz.sweep.start = 0;// (float)atof(optarg);
        // fin = strchr(optarg, '_');
        // fin = fin + 1;
        app_task->effect->cutoff_freq_Hz.sweep.end = 0; //(float)atof(fin);
        // printf("prm.cutoff_freq.sweep.start %f\n", app_task->effect->prm.cutoff_freq.sweep.start );
        // printf("app_task->effect->prm.cutoff_freq.sweep.end %f\n", app_task->effect->prm.cutoff_freq.sweep.end);
        break;
      case 'q':
          printf(" PCM\n");
          app_task->audioFormatType = PCM;
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

  c = app_task_check(app_task);
  if(c != 0){
    exit (EXIT_FAILURE);
  }
  app_handl(app_task);

  if(app_task->generator){
    free(app_task->generator);
  }
  if(app_task->input){
    free(app_task->input_f_path);
  }
  if(app_task->ouput){
    free(app_task->output_f_path);
  }

  exit (EXIT_SUCCESS);
}


static void print_help(void){
  printf("%s",HELP_TEXT_OPT);
  printf("%s",HELP_TEXT_IN);
  printf("%s",HELP_TEXT_OUT);
  printf("%s",HELP_TEXT_P_HDR);
  printf("%s",HELP_TEXT_GAIN);
  printf("%s",HELP_TEXT_FIR_BPF);
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

  printf("%s",HELP_TEXT_IIR);
  printf("%s",HELP_TEXT_IIR2);
  printf("%s",HELP_TEXT_IIR_T);
  printf("%s",HELP_TEXT_IIR_LPF);
  printf("%s",HELP_TEXT_IIR_HPF);
  printf("%s",HELP_TEXT_IIR_BPF);
  printf("%s",HELP_TEXT_IIR_BSF);
  printf("%s",HELP_TEXT_IIR_PQE);
  printf("%s",HELP_TEXT_IIR_LSH);
  printf("%s",HELP_TEXT_IIR_HSH);

    printf("%s",HELP_TEXT_CROSSOVER);
}

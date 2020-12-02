#include "test_app.h"

static void print_help(void);

int main (int argc, char **argv)
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
        {"in",        required_argument, 0, 'i'},
        {"out",       required_argument, 0, 'o'},
        {"generator", required_argument, 0, 'g'},
        {"effect",    no_argument,       0, 'e'},
        {"PMC",       no_argument,       0, 't'},
        {0, 0, 0, 0}
      };
    /* getopt_long stores the option index here. */
    int option_index = 0;

    c = getopt_long (argc, argv, "chepqfo:g:i:b:r:s:",
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
      case 'i':
        app_task->input_f_path = (char *)malloc(strlen(optarg) + 1);
        memcpy(app_task->input_f_path, optarg, (strlen(optarg) + 1));
        break;
      case 'o':
        app_task->output_f_path = malloc(strlen(optarg) + 1);
        memcpy(app_task->output_f_path, optarg, (strlen(optarg) + 1));
        break;
      case 'g':
        app_task->generator = app_parse_gen_opt(optarg);
        if(!app_task->generator){
          printf("error");
        }
        break;
      case 'e':
        app_task->effect = true;
        printf("SCDCSCDS\n");
        break;
      case 't':
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
  if(app_task->input_f_path){
    free(app_task->input_f_path);
  }
  if(app_task->output_f_path){
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

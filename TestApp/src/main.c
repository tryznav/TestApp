#include "test_app.h"

static int verbose_flag;

main (int argc, char **argv)
{
  int c;
  app_func_t *app_task = (app_func_t *)malloc(sizeof(app_func_t));
  memset(app_task,0, sizeof(app_func_t));

  while (1){
    static struct option long_options[] =
      {
        /* These options set a flag. */
        {"verbose", no_argument,       &verbose_flag, 1},
        {"brief",   no_argument,       &verbose_flag, 0},
        /* These options don’t set a flag.
            We distinguish them by their indices. */
        {"add",       no_argument,       0, 'a'},
        {"append",    no_argument,       0, 'b'},
        {"hdr",       no_argument,       0, 'h'},
        {"gain",      required_argument, 0, 'g'},
        {"in",        required_argument, 0, 'i'},
        {"out",       required_argument, 0, 'o'},
        {0, 0, 0, 0}
      };
    /* getopt_long stores the option index here. */
    int option_index = 0;

    c = getopt_long (argc, argv, "abho:g:i:",
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

      case 'a':
        puts ("option -a\n");
        break;
      case 'b':
        puts ("option -b\n");
        break;
      case 'h':
        app_task->print_hdr = true;
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
        app_task->output_f_path = (char *)malloc(strlen(optarg) + 1);
        memcpy(app_task->output_f_path, optarg, (strlen(optarg) + 1));
        break;
      case '?':
        /* getopt_long already printed an error message. */
        break;

      default:
          fprintf(stderr, RED "Error:\t"RESET BOLDWHITE"Unknown option. Rejected\n"RESET);
      }
  }




  /* Instead of reporting ‘--verbose’
     and ‘--brief’ as they are encountered,
     we report the final status resulting from them. */
  if (verbose_flag)
    puts ("verbose flag is set");

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
#include "test_app.h"
#include  <float.h>

int app_task_check(app_func_t *task){
    if(task->generator && task->output_f_path){
        printf("asdasd\n");      
        return 0;
    }
    if(task->effect && task->input_f_path && task->output_f_path){
        return 0;
    }
    if(task->input_f_path && task->print_hdr){
        return 0;
    }

    if(task->input_f_path && task->output_f_path){
        return 0;
    }
    fprintf(stderr,RED "Not enough arguments. "BOLDWHITE"Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET);
    return -1;
}

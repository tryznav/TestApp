#include "test_app.h"
#include  <float.h>

static void print_hdr(char *input_f_path);

int app_task_check(app_func_t *task){
    if(task->generator && task->ouput){        
        return 0;
    }
    if(task->effect && task->input && task->ouput){
        return 0;
    }
    if(task->input && task->print_hdr){
        return 0;
    }

    if(task->input && task->ouput){
        return 0;
    }
    fprintf(stderr,RED "Not enough arguments. "BOLDWHITE"Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET);
    return -1;
}

// static void print_hdr(char *input_f_path){
//     RiffChunk_t     *RiffChunk = NULL;
//     FmtChunk_t      *FmtChunk = NULL;
//     DataChunk_t     *DataChunk = NULL; 
//     void            *file_buff = NULL;
//     void            *dat = NULL;

//     fhand_parse_wav_file(input_f_path, RiffChunk, FmtChunk,  DataChunk, dat, file_buff);

//     fhand_print_chunk(RiffChunk, FmtChunk, DataChunk);

//     if(file_buff != NULL){
//         free(file_buff);
//     }

//     RiffChunk = NULL;
//     FmtChunk = NULL;
//     DataChunk = NULL;
//     //FmtChunk->audioFormat == 1;
// }

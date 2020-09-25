#include "test_app.h"
#include  <float.h>

static void print_hdr(char *input_f_path);
static void *conv_aud_for_eff(FmtChunk_t *FmtChunk, RiffChunk_t *RiffChunk, DataChunk_t *DataChunk, size_t samples_coun);

int app_handler(app_func_t *task){
    if(task->generator && task->ouput){        
        pross_waw_t pross_waw;
        pross_waw.efect = NULL;
        pross_waw.gen_sig = NULL;
        app_generator_init(&pross_waw, task);
        if(task->gain){
            app_gain_init(&pross_waw, NULL, task->output_f_path, task->gain_dB);
        }
        fhand_wav_process(&pross_waw);
        app_gain_del(&pross_waw);
        app_gen_sig_del(&pross_waw);
        printf(BOLDGREEN"Audio data is generated\n"RESET);
        return 0;
    }

    if(task->gain && task->input && task->ouput){
        app_gain(task->input_f_path, task->output_f_path, task->gain_dB);
        printf(BOLDGREEN"Audio data is generated\n"RESET);
        return 0;
    }
    if(task->input && task->print_hdr){
        print_hdr(task->input_f_path);
        printf(BOLDGREEN"Audio file of length 10ms created\n"RESET);
        return 0;
    }

    if(task->input && task->ouput){
        pross_waw_t pross_waw;
        pross_waw.gen_sig = NULL;
        pross_waw.efect = NULL;
        pross_waw.buff.size_ms = 10;
        pross_waw.dest_f_path = task->output_f_path;
        pross_waw.src_f_path = task->input_f_path;
        fhand_wav_process(&pross_waw);

        printf(BOLDGREEN"Audio file of length 10ms created\n"RESET);
        return 0;
    }
    fprintf(stderr,RED "Not enough arguments. "BOLDWHITE"Specify"BOLDYELLOW" --help"BOLDWHITE" for usage\n"RESET);
    return -1;
}

static void print_hdr(char *input_f_path){
    RiffChunk_t     *RiffChunk = NULL;
    FmtChunk_t      *FmtChunk = NULL;
    DataChunk_t     *DataChunk = NULL; 
    void            *file_buff = NULL;
    void            *dat = NULL;

    fhand_parse_wav_file(input_f_path, RiffChunk, FmtChunk,  DataChunk, dat, file_buff);

    fhand_print_chunk(RiffChunk, FmtChunk, DataChunk);

    if(file_buff != NULL){
        free(file_buff);
    }

    RiffChunk = NULL;
    FmtChunk = NULL;
    DataChunk = NULL;
    //FmtChunk->audioFormat == 1;
}

static void *conv_aud_for_eff(FmtChunk_t *FmtChunk, RiffChunk_t *RiffChunk, DataChunk_t *DataChunk, size_t samples_count){
    float       *file_buff = (float *)malloc(sizeof(float) * samples_count * 2);
    int16_t     *data_int16 = NULL;

    if(FmtChunk->audioFormat == 1){
        data_int16 = (int16_t *)(DataChunk + sizeof(DataChunk_t));
        for(size_t i = 0; i < samples_count; i++){
            file_buff[i] = (float)data_int16[i]; 
        }
    }
    if(FmtChunk->audioFormat == 3){
        memcpy(file_buff, (DataChunk + sizeof(DataChunk_t)), samples_count * 2 * sizeof(float));
    }
    return file_buff;
}

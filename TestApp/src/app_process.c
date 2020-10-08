#include "test_app.h"

int app_handl(app_func_t *task){
    wav_hdr_t           hdr         = {NULL, NULL, NULL};
    int32_t             n           = 0;
    pross_waw_t         pr;
    
    pr.efect = NULL;
    pr.gen_sig = NULL;

    pr.buff.size_ms = 10;
    pr.dest_f_path = task->output_f_path;
    pr.src_f_path = task->input_f_path;

    if(task->generator){
        app_generator_init(&pr, task);
        n = fhand_newhdr_f(&hdr, pr.gen_sig->sample_rate, pr.gen_sig->length_sample);
    }else{
        if((pr.src_file = fhand_parse_wav_file_fptr(pr.src_f_path, &hdr)) == NULL){
            fprintf(stderr,RED"Error: "BOLDWHITE"File unparse. Reject\n"RESET);
            return -1;
        }
        if((n = check(pr ,&hdr)) != 0){
            free_chunk_hdr(&hdr);
            fclose(pr.src_file);
            return -1;
        }
    }

    set_proc_prm(&pr, &hdr, task);

    if((pr.dest_file = fhand_newav(pr.dest_f_path, &hdr)) == NULL){
        fprintf(stderr, RED"Error: "BOLDWHITE"File uncreate. Reject\n"RESET);
        free_chunk_hdr(&hdr);
        fclose(pr.src_file);
        return -1;
    }

    if(pr.gen_sig == NULL){

    }else{
        
    }

}

/****************************************************************************************/

static int32_t set_proc_prm(pross_waw_t *pr, wav_hdr_t *hdr, app_func_t *task){
    int32_t n = -1;
    hdr->FmtChunk->chunkSize =  sizeof(FmtChunk_t) - 8;
    //расчет размера буффера 
    pr->buff.size_byte = ((hdr->FmtChunk->sampleRate/1000)*pr->buff.size_ms)*hdr->FmtChunk->blockAlign;
    pr->audio.size_byte = (size_t)hdr->DataChunk->chunkSize;
    pr->buff.samples = pr->buff.size_byte / hdr->FmtChunk->blockAlign;
    if ((pr->audio.size_byte % pr->audio.size_byte) != 0){
       hdr->DataChunk->chunkSize = ((pr->audio.size_byte / pr->audio.size_byte) + 1) * pr->buff.size_byte;
    }
    hdr->RiffChunk->chunkSize = sizeof(RiffChunk_t) - 8 + sizeof(FmtChunk_t) + sizeof(DataChunk_t) + hdr->DataChunk->chunkSize;

    if(task->effect){
        n = app_efect_init(pr, hdr, task);
        if(n != 0){
            fprintf(stderr,RED"Error: "BOLDWHITE"reset_coeffs_wav\n"RESET);// пересмотреть чистый ли HEAP
            return -1;
        }
    }


    return 0;
}

/********************************************************************************************************************************************************/

int32_t efect_init(pross_waw_t *pr, wav_hdr_t  *hdr, app_func_t *task){
    size_t     states_bytes;
    int32_t    n;

    n = pr->efect->effect_process_get_sizes(&states_bytes);
    if(n != 0){
        return n;
    }

    pr->efect->states = malloc(states_bytes);
    if(pr->efect->states == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    n = pr->efect->effect_control_initialize(pr->efect->params, pr->efect->coeffs,  hdr->FmtChunk->sampleRate);
    if(n != 0){
        return n;
    }

    n = pr->efect->effect_update_coeffs((const void*)pr->efect->params, pr->efect->coeffs);
    if(n != 0){
        return n;
    }

    return 0;
}
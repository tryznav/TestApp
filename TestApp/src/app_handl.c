#include "test_app.h"

static int32_t del_proc_prm(pross_waw_t *pr);
static int32_t process(pross_waw_t *pr);
static int32_t check( pross_waw_t *pr, wav_hdr_t *hdr);
static int32_t set_handl_prm(pross_waw_t *pr, wav_hdr_t *hdr, app_func_t *task);

int32_t app_handl(app_func_t *task){
    wav_hdr_t           hdr         = {NULL, NULL, NULL};
    int32_t             n           = 0;
    pross_waw_t         pr;
    
    pr.efect = NULL;
    pr.gen_sig = NULL;

    pr.buff.size_ms = 10;
    pr.dest_f_path = task->output_f_path;
    pr.src_f_path = task->input_f_path;

    if(task->generator){
        app_generator_init(&pr, task->generator, task->audioFormatType);
        n = fhand_newhdr_f(&hdr, pr.gen_sig->sample_rate, pr.gen_sig->length_sample, task->audioFormatType);
    }else{
        if((pr.src_file = fhand_parse_wav_file_fptr(pr.src_f_path, &hdr)) == NULL){
            fprintf(stderr,RED"Error: "BOLDWHITE"File unparse. Reject\n"RESET);
            return -1;
        }
        if((n = check(&pr ,&hdr)) != 0){
            free_chunk_hdr(&hdr);
            fclose(pr.src_file);
            return -1;
        }
    }


    set_handl_prm(&pr, &hdr, task);

        

    if((pr.dest_file = fhand_newav(pr.dest_f_path, &hdr)) == NULL){
        fprintf(stderr, RED"Error: "BOLDWHITE"File uncreate. Reject\n"RESET);
          printf(BOLDGREEN"DONE\n"RESET);
        free_chunk_hdr(&hdr);
        fclose(pr.src_file);
        return -1;
    }


    free_chunk_hdr(&hdr);

    process(&pr);

    del_proc_prm(&pr);
    printf(BOLDGREEN"DONE\n"RESET);

    return 0;
}

/****************************************************************************************/


static int32_t process(pross_waw_t *pr){
    size_t n = 0;
    void *buff = malloc(pr->buff.size_byte);
    if (buff == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    memset(buff, 0, pr->buff.size_byte);

    while (pr->audio.size_byte > pr->buff.size_byte){

        if(pr->gen_sig == NULL){
            fread(buff, 1, pr->buff.size_byte, pr->src_file);
        }else{
            pr->gen_sig->tsig_gen_sig_st(pr->buff.samples, pr->gen_sig->states, buff);
        }

        if(pr->efect != NULL){
            pr->efect->effect_process(pr->efect->coeffs, pr->efect->states, buff, pr->buff.samples);
        }

        n = fwrite((const void *)buff, 1,  pr->buff.size_byte, pr->dest_file);
        if(n != pr->buff.size_byte){
            fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
            exit(EXIT_FAILURE);
        }

        pr->audio.size_byte -= pr->buff.size_byte;
    }
    if(pr->audio.size_byte != 0){
        memset(buff, 0, pr->buff.size_byte);
        if(pr->gen_sig == NULL){
            fread(buff, 1, pr->audio.size_byte, pr->src_file);
        }else{
            pr->gen_sig->tsig_gen_sig_st(pr->buff.samples, pr->gen_sig->states, buff);
        }

        if(pr->efect != NULL){
            pr->efect->effect_process(pr->efect->coeffs, pr->efect->states, buff, pr->buff.samples);
        }

        n = fwrite((const void *)buff, 1,  pr->buff.size_byte, pr->dest_file);
        if(n != pr->buff.size_byte){
            fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
            exit(EXIT_FAILURE);
        }

    }

    free(buff);
    buff = NULL;
    return 0;
}

/****************************************************************************************/

static int32_t set_handl_prm(pross_waw_t *pr, wav_hdr_t *hdr, app_func_t *task){
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
        n = app_efect_init(pr, hdr, task->effect);
        if(n != 0){
            fprintf(stderr,RED"Error: "BOLDWHITE"reset_coeffs_wav\n"RESET);// пересмотреть чистый ли HEAP
            return -1;
        }
    }


    return 0;
}

/********************************************************************************************************************************************************/

static int32_t check( pross_waw_t *pr, wav_hdr_t *hdr){
    if(hdr->FmtChunk->numChannels != STEREO_DATA) {
        fprintf(stderr,RED"Error: Wrong audio format. "BOLDWHITE"App support stereo data.\n"RESET);
        return -1;
    }
    if(hdr->FmtChunk->audioFormat != PCM && hdr->FmtChunk->audioFormat != IEEE_754){
        fprintf(stderr,RED"Error: Wrong audio format. "BOLDWHITE"App support PCM and IEEE 754 formats.\n"RESET);
        return -1;
    }
    if(pr->efect){
        if(hdr->FmtChunk->audioFormat == PCM){
            fprintf(stderr,RED"Error: Wrong audio format. "BOLDWHITE"Effect support IEEE 754 formats.\n"RESET);
            return -1;
        }
        if(pr->efect->coeffs == NULL || pr->efect->params == NULL){
            fprintf(stderr,RED"Error: params. "BOLDWHITE"No params pre-alloc.\n"RESET);
            return -1;
        }
    }

    return 0;
}

static int32_t del_proc_prm(pross_waw_t *pr){
    if(pr->gen_sig){
        if(pr->gen_sig->states){
            free(pr->gen_sig->states);
            pr->gen_sig->states = NULL;
        }
    } else {
        fclose(pr->src_file);
    }
    
    if(pr->efect){
        if(pr->efect->states){
            free(pr->efect->states);
            pr->efect->states = NULL;
        }
    }
    fclose(pr->dest_file);
    return 0;
}

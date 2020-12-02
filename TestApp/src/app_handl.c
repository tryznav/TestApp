#include "test_app.h"

static int32_t del_proc_prm(pross_waw_t *pr);
static int32_t process(pross_waw_t *pr);
static int32_t check(pross_waw_t *pr, wav_hdr_t *hdr);
static int32_t set_handl_prm(pross_waw_t *pr, wav_hdr_t *hdr, app_func_t *task);

int32_t app_handl(app_func_t *task){
    wav_hdr_t           hdr         = {NULL, NULL, NULL};
    int32_t             n           = 0;
    pross_waw_t         pr;
    
    pr.effect = NULL;
    pr.gen_sig = NULL;

    if(task->generator){
        app_generator_init(&pr, task->generator, task->audioFormatType);
        n = fhand_newhdr_f(&hdr, pr.gen_sig->sample_rate, pr.gen_sig->length_sample, task->audioFormatType);
    }else{
        if((pr.src_file = fhand_parse_wav_file_fptr(task->input_f_path, &hdr)) == NULL){
            fprintf(stderr, RED"Error: "BOLDWHITE"File unparse. Reject\n"RESET);
            return -1;
        }
        if((n = check(&pr ,&hdr)) != 0){
            free_chunk_hdr(&hdr);
            fclose(pr.src_file);
            return -1;
        }
    }

    set_handl_prm(&pr, &hdr, task);
    // printf("task->input_f_path %s\n", );
    if((pr.dest_file = fhand_newav(task->output_f_path, &hdr)) == NULL){
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
        // printf("PROOOOOSSSSSSSCCCCCCC = %d\n", pr->buff.size_byte);
    // void *buff = aligned_alloc(16, pr->buff.size_byte);
    void *buff = malloc(pr->buff.size_byte);
    if (buff == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    memset(buff, 0, pr->buff.size_byte);
    printf("pr->buff.size_byte = %d\n", pr->buff.size_byte);


    while (pr->audio.size_byte > pr->buff.size_byte){

        if(pr->gen_sig == NULL){
            fread(buff, 1, pr->buff.size_byte, pr->src_file);
        }else{
            pr->gen_sig->tsig_gen_sig_st(pr->buff.samples, pr->gen_sig->states, buff);
        }

        if(pr->effect){


            effect_process(pr->effect->coeffs, pr->effect->states, buff, pr->buff.samples);
      
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

        if(pr->effect != NULL){
            effect_process(pr->effect->coeffs, pr->effect->states, buff, pr->buff.samples);
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
    hdr->FmtChunk->chunkSize = sizeof(FmtChunk_t) - 8;
    //расчет размера буффера 
    pr->buff.size_byte = ((hdr->FmtChunk->sampleRate/1000) * SIZE_MS)*hdr->FmtChunk->blockAlign;
    pr->audio.size_byte = (size_t)hdr->DataChunk->chunkSize;
    pr->buff.samples = pr->buff.size_byte / hdr->FmtChunk->blockAlign;
    if ((pr->audio.size_byte % pr->audio.size_byte) != 0){
       hdr->DataChunk->chunkSize = ((pr->audio.size_byte / pr->audio.size_byte) + 1) * pr->buff.size_byte;
    }
    hdr->RiffChunk->chunkSize = sizeof(RiffChunk_t) - 8 + sizeof(FmtChunk_t) + sizeof(DataChunk_t) + hdr->DataChunk->chunkSize;

    if(task->effect){
        n = app_efect_init(pr, hdr->FmtChunk->sampleRate);
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
    if(pr->effect){
        if(hdr->FmtChunk->audioFormat == EFFECT_AUDIO_FORMAT){
            fprintf(stderr,RED"Error: Wrong audio format. "BOLDWHITE"Effect support IEEE 754 formats.\n"RESET);
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
    
    if(pr->effect){
        if(pr->effect->states){
            free(pr->effect->states);
            pr->effect->states = NULL;
        }
        if (pr->effect->coeffs) {
            free(pr->effect->coeffs);
            pr->effect->coeffs = NULL;
        }
    }
    fclose(pr->dest_file);
    return 0;
}

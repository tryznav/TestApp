#include "file_handler.h"

static int32_t check( pross_waw_t *pross_waw, wav_hdr_t *hdr);
static int32_t set_proc_prm(pross_waw_t *pr, wav_hdr_t *hdr);
static int32_t reset_efect_coeffs_wav(pross_waw_t *pr, wav_hdr_t  *hdr);
static int32_t process(pross_waw_t *pr);
static int32_t del_proc_prm(pross_waw_t *pr);

int32_t fhand_wav_process(pross_waw_t *pr){
    wav_hdr_t           hdr         = {NULL, NULL, NULL};
    int32_t             n           = 0;

    if(pr->gen_sig == NULL){
        if((pr->src_file = fhand_parse_wav_file_fptr(pr->src_f_path, &hdr)) == NULL){
            fprintf(stderr,RED"Error: "BOLDWHITE"File unparse. Reject\n"RESET);
            return -1;
        }
        if((n = check(pr ,&hdr)) != 0){
            free_chunk_hdr(&hdr);
            fclose(pr->src_file);
            return -1;
        }
    }else{
        n = fhand_newhdr_f(&hdr, pr->gen_sig->sample_rate, pr->gen_sig->length_sample);
    }

    set_proc_prm(pr, &hdr);
    if((pr->dest_file = fhand_newav(pr->dest_f_path, &hdr)) == NULL){
        fprintf(stderr, RED"Error: "BOLDWHITE"File uncreate. Reject\n"RESET);
        free_chunk_hdr(&hdr);
        fclose(pr->src_file);
        return -1;
    }

    free_chunk_hdr(&hdr);
    process(pr);
    del_proc_prm(pr);

    if(pr->gen_sig == NULL){
        fclose(pr->src_file);
    }

    fclose(pr->dest_file);

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
            pr->gen_sig->tsig_gen_sig_st(pr->gen_sig->sample_rate, pr->buff.samples, pr->gen_sig->amplitude_coef, pr->gen_sig->params, pr->gen_sig->states, buff);
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
            pr->gen_sig->tsig_gen_sig_st(pr->gen_sig->sample_rate, pr->buff.samples, pr->gen_sig->amplitude_coef, pr->gen_sig->params, pr->gen_sig->states, buff);
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

static int32_t set_proc_prm(pross_waw_t *pr, wav_hdr_t *hdr){
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

    if(pr->efect){
        n = reset_efect_coeffs_wav(pr, hdr);
        if(n != 0){
            fprintf(stderr,RED"Error: "BOLDWHITE"reset_coeffs_wav\n"RESET);// пересмотреть чистый ли HEAP
            return -1;
        }
    }

    if(pr->gen_sig){
        pr->gen_sig->states = pr->gen_sig->tsig_sig_init_states(pr->gen_sig->sample_rate, pr->gen_sig->length_sample, pr->gen_sig->params);
        if(pr->gen_sig->states == NULL){
            fprintf(stderr,RED"Error: "BOLDWHITE"tsig_sig_init_states\n"RESET); // пересмотреть чистый ли HEAP
            return -1;
        }
    }

    return 0;
}

/********************************************************************************************************************************************************/

static int32_t reset_efect_coeffs_wav(pross_waw_t *pr, wav_hdr_t  *hdr){
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

/****************************************************************************************/

static int32_t check( pross_waw_t *pr, wav_hdr_t *hdr){
    if(hdr->FmtChunk->numChannels != STEREO_DATA) {
        fprintf(stderr,RED"Error: Wrong audio format. "BOLDWHITE"App support stereo data.\n"RESET);
        return -1;
    }
    if(hdr->FmtChunk->audioFormat != PMC && hdr->FmtChunk->audioFormat != IEEE_754){
        fprintf(stderr,RED"Error: Wrong audio format. "BOLDWHITE"App support PMC and IEEE 754 formats.\n"RESET);
        return -1;
    }
    if(pr->efect){
        if(hdr->FmtChunk->audioFormat == PMC){
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

//*******************************************************************************************************************/

static int32_t del_proc_prm(pross_waw_t *pr){
    if(pr->gen_sig){
        if(pr->gen_sig->states){
            free(pr->gen_sig->states);
            pr->gen_sig->states = NULL;
        }
    }
    if(pr->efect){
        if(pr->efect->states){
            free(pr->efect->states);
            pr->efect->states = NULL;
        }
    }
    return 0;
}

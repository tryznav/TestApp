#include "file_handler.h"

static int32_t check(wav_hdr_t *hdr, pross_waw_t *pross_waw);
static int set_proc_prm(pross_waw_t *pross_waw, process_waw_hand_t *proc, wav_hdr_t *hdr);
static int32_t reset_coeffs_wav(pross_waw_t *pross_waw, process_waw_hand_t *proc, wav_hdr_t  *hdr);
static int process(process_waw_hand_t *proc);


int32_t fhand_wav_process(pross_waw_t *pross_waw){
    process_waw_hand_t  proc;
    wav_hdr_t           hdr         = {NULL, NULL, NULL};
    int32_t             n           = 0;

    if(pross_waw->gen_sig == NULL){
        if((proc.src_file = fhand_parse_wav_file_fptr(pross_waw->src_f_path, &hdr)) == NULL){
            fprintf(stderr,RED"Error: "BOLDWHITE"File unparse. Reject\n"RESET);
            return -1;
        }
        if((n = check(&hdr, pross_waw)) != 0){
            free_chunk_hdr(&hdr);
            fclose(proc.src_file);
            return -1;
        }
    }else{
        n = fhand_newhdr_f(&hdr, pross_waw->gen_sig->sample_rate, pross_waw->gen_sig->length_sample);
    }

    set_proc_prm(pross_waw, &proc, &hdr);
    if((proc.dest_file = fhand_newav(pross_waw->dest_f_path, &hdr)) == NULL){
        fprintf(stderr, RED"Error: "BOLDWHITE"File uncreate. Reject\n"RESET);
        free_chunk_hdr(&hdr);
        fclose(proc.src_file);
        return -1;
    }

    free_chunk_hdr(&hdr);
    process(&proc);

    if(pross_waw->gen_sig == NULL){
        fclose(proc.src_file);
    }

    fclose(proc.dest_file);

    return 0;

}

/****************************************************************************************/

 static int process(process_waw_hand_t *proc){
    size_t n = 0;
    void *audio = malloc(proc->sizeNms);
    if (audio == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    memset(audio, 0, proc->sizeNms);
    while (proc->allAudiosize > proc->sizeNms){

        if(proc->gen_sig == NULL){
            fread(audio, 1, proc->sizeNms, proc->src_file);
        }else{
            proc->gen_sig->tsig_gen_sig_st(proc->gen_sig->sample_rate, proc->samples_count, proc->gen_sig->amplitude_coef,proc->gen_sig->params,proc->gen_sig->states, audio);
        }
        if(proc->coeffs != NULL){
            proc->effect_process(proc->coeffs, proc->states, audio, proc->samples_count);
        }
        n = fwrite((const void *)audio, 1,  proc->sizeNms, proc->dest_file);
        if(n != proc->sizeNms){
            fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        proc->allAudiosize -= proc->sizeNms;
    }
    if(proc->allAudiosize != 0){
        memset(audio, 0, proc->sizeNms);
        if(proc->gen_sig == NULL){
            fread(audio, 1, proc->sizeNms, proc->src_file);
        }else{
            proc->gen_sig->tsig_gen_sig_st(proc->gen_sig->sample_rate, proc->samples_count, proc->gen_sig->amplitude_coef,proc->gen_sig->params,proc->gen_sig->states, audio);
        }
        if(proc->coeffs != NULL){
            proc->effect_process(proc->coeffs, proc->states, audio, proc->samples_count);
        }

        n = fwrite((const void *)audio, 1,  proc->sizeNms, proc->dest_file);
        if(n != proc->sizeNms){
            fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
            exit(EXIT_FAILURE);
        }

    }
    if(proc->gen_sig == NULL){
        free(proc->gen_sig->states);
        proc->gen_sig->states = NULL;
    }
    if(proc->coeffs != NULL){

    }
    free(audio);
    audio = NULL;
    return 0;
}

/****************************************************************************************/

static int set_proc_prm(pross_waw_t *pross_waw, process_waw_hand_t *proc, wav_hdr_t *hdr){
    int32_t n = -1;
    hdr->FmtChunk->chunkSize =  sizeof(FmtChunk_t) - 8;
    proc->sizeNms = ((hdr->FmtChunk->sampleRate/1000)*pross_waw->size_ms)*hdr->FmtChunk->blockAlign;
    proc->allAudiosize = (size_t)hdr->DataChunk->chunkSize;
    if ((proc->allAudiosize % proc->sizeNms) == 0){
        proc->samples_count = proc->sizeNms / hdr->FmtChunk->blockAlign;
        hdr->DataChunk->chunkSize = hdr->DataChunk->chunkSize;
    } else {
        proc->samples_count = (proc->allAudiosize / proc->sizeNms) + 1;
        hdr->DataChunk->chunkSize = proc->samples_count * proc->sizeNms;
        proc->samples_count = proc->sizeNms / hdr->FmtChunk->blockAlign;
    }
    hdr->RiffChunk->chunkSize = sizeof(RiffChunk_t) - 8 + sizeof(FmtChunk_t) + sizeof(DataChunk_t) + hdr->DataChunk->chunkSize;
    proc->coeffs = pross_waw->coeffs;

    if(pross_waw->coeffs != NULL){
        proc->effect_process = pross_waw->effect_process;
        n = reset_coeffs_wav(pross_waw, proc, hdr);
        if(n != 0){
            fprintf(stderr,RED"Error: "BOLDWHITE"reset_coeffs_wav\n"RESET);// пересмотреть чистый ли HEAP
            return -1;
        }
    }

    proc->gen_sig = pross_waw->gen_sig;
    if(proc->gen_sig){
        proc->gen_sig->states = proc->gen_sig->tsig_sig_init_states(proc->gen_sig->sample_rate, proc->gen_sig->length_sample, proc->gen_sig->params);
        if(proc->gen_sig->states == NULL){
            fprintf(stderr,RED"Error: "BOLDWHITE"tsig_sig_init_states\n"RESET); // пересмотреть чистый ли HEAP
            return -1;
        }
    }

    return 0;
}

/********************************************************************************************************************************************************/

int32_t reset_coeffs_wav(pross_waw_t *pross_waw, process_waw_hand_t *proc, wav_hdr_t    *hdr){
    size_t     states_bytes;
    int32_t    n;

    n = pross_waw->effect_process_get_sizes(&states_bytes);
    if(n != 0){
        return n;
    }

    proc->states = malloc(sizeof(process_waw_hand_t));
    if(proc->states == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    n = pross_waw->effect_control_initialize(pross_waw->params,pross_waw->coeffs,  hdr->FmtChunk->sampleRate);
    if(n != 0){
        return n;
    }

    n = pross_waw->effect_update_coeffs(pross_waw->params, pross_waw->coeffs);
    if(n != 0){
        return n;
    }

    proc->coeffs = (void const*)pross_waw->coeffs;
    return 0;

}

/****************************************************************************************/

static int32_t check(wav_hdr_t *hdr, pross_waw_t *pross_waw){
    if(hdr->FmtChunk->numChannels != STEREO_DATA) {
        fprintf(stderr,RED"Error: Wrong audio format. "BOLDWHITE"App support stereo data.\n"RESET);
        return -1;
    }
    if(hdr->FmtChunk->audioFormat != PMC && hdr->FmtChunk->audioFormat != IEEE_754){
        fprintf(stderr,RED"Error: Wrong audio format. "BOLDWHITE"App support PMC and IEEE 754 formats.\n"RESET);
        return -1;
    }
    if(hdr->FmtChunk->audioFormat == PMC && pross_waw->coeffs != NULL){
        fprintf(stderr,RED"Error: Wrong audio format. "BOLDWHITE"Effect support IEEE 754 formats.\n"RESET);
        return -1;
    }
    if(pross_waw->coeffs != NULL && pross_waw->coeffs == NULL){
        fprintf(stderr,RED"Error: params. "BOLDWHITE"No params pre-alloc.\n"RESET);
        return -1;
    }
    return 0;
}

//*******************************************************************************************************************/

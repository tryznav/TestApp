#include "file_handler.h"

static int32_t check(wav_hdr_t *hdr, pross_waw_t *pross_waw);
static int set_proc_prm(pross_waw_t *pross_waw, process_waw_hand_t *proc, wav_hdr_t *hdr);
static int32_t reset_coeffs_wav(pross_waw_t *pross_waw, process_waw_hand_t *proc, wav_hdr_t  *hdr);
static int process(process_waw_hand_t *proc);


int32_t fhand_wav_process(pross_waw_t *pross_waw){
    process_waw_hand_t  proc;
    wav_hdr_t           hdr         = {NULL, NULL, NULL};
    int32_t             n           = 0;

    // proc = (process_waw_hand_t *)malloc(sizeof(process_waw_hand_t));
    // if(proc == NULL){
    //     fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
    //     exit(EXIT_FAILURE);
    // }

    if((proc.src_file = fhand_parse_wav_file_fptr(pross_waw->src_f_path, &hdr)) == NULL){
        fprintf(stderr,RED"Error: "BOLDWHITE"File unparse. Reject\n"RESET);
        return -1;
    }
    fhand_print_chunk(hdr.RiffChunk, hdr.FmtChunk, hdr.DataChunk);
    if((n = check(&hdr, pross_waw)) != 0){
        free_chunk_hdr(&hdr);
        fclose(proc.src_file);
        return -1;
    }

    set_proc_prm(pross_waw, &proc, &hdr);

    if((proc.dest_file = fhand_newav(pross_waw->dest_f_path, &hdr)) == NULL){
        fprintf(stderr, RED"Error: "BOLDWHITE"File uncreate. Reject\n"RESET);
        free_chunk_hdr(&hdr);
        fclose(proc.src_file);
        return -1;
    }
    //fhand_print_chunk(hdr.RiffChunk, hdr.FmtChunk, hdr.DataChunk);

    //calculate

    free_chunk_hdr(&hdr);

    process(&proc);

    fclose(proc.src_file);
    fclose(proc.dest_file);

    // free(proc);
    return 0;

}

/****************************************************************************************/

int process(process_waw_hand_t *proc){
    void *audio = malloc(proc->sizeNms);
    if (audio == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    memset(audio, 0, proc->sizeNms);

    while (proc->allAudiosize > proc->sizeNms){
        fread(audio, 1, proc->sizeNms, proc->src_file);
        if(proc->coeffs != NULL){
            proc->effect_process(proc->coeffs, proc->states, audio, proc->samples_count);
        }
        fwrite(audio, 1,  proc->sizeNms, proc->dest_file);
        proc->allAudiosize -= proc->sizeNms;
    }

    if(proc->allAudiosize != 0){
        memset(audio, 0, proc->sizeNms);
        fread(audio, 1, proc->allAudiosize, proc->src_file);
        if(proc->coeffs != NULL){
            proc->effect_process(proc->coeffs, proc->states, audio, proc->samples_count);
        }
        fwrite(audio, 1,  proc->sizeNms, proc->dest_file);
    }

    return 0;
}

/****************************************************************************************/

float audio_convert_int16_to_float(){
    return 0;
}

/****************************************************************************************/

static int set_proc_prm(pross_waw_t *pross_waw, process_waw_hand_t *proc, wav_hdr_t *hdr){
    int32_t n = -1;
    hdr->FmtChunk->chunkSize =  sizeof(FmtChunk_t) - 8;
    proc->sizeNms = ((hdr->FmtChunk->sampleRate/1000)*pross_waw->size_ms)*hdr->FmtChunk->blockAlign;

    proc->allAudiosize = (size_t)hdr->DataChunk->chunkSize;
    if ((proc->allAudiosize % proc->sizeNms) == 0){
        proc->samples_count = proc->allAudiosize / proc->sizeNms;
        hdr->DataChunk->chunkSize = hdr->DataChunk->chunkSize;
    } else {
        proc->samples_count = (proc->allAudiosize / proc->sizeNms) + 1;
        hdr->DataChunk->chunkSize = proc->samples_count * proc->sizeNms;
    }
    hdr->RiffChunk->chunkSize = sizeof(RiffChunk_t) - 8 + sizeof(RiffChunk_t) + sizeof(DataChunk_t) + hdr->DataChunk->chunkSize;

    proc->coeffs = pross_waw->coeffs;

    
    if(pross_waw->coeffs != NULL){
        proc->effect_process = pross_waw->effect_process;
        n = reset_coeffs_wav(pross_waw, proc, hdr);
        if(n != 0){
            fprintf(stderr,RED"Error: "BOLDWHITE"reset_coeffs_wav\n"RESET);
        }
    }
    
    return 0;
}

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

    // no param  from wav now 
    //n = pross_waw->effect_set_parameter()
    // if(n != 0){
    //     return n;
    // }

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


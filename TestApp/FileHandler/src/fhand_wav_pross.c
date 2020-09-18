#include "file_handler.h"

int fhand_wav_pross(pross_waw_t *pross_waw){
    pross_waw_hand_t    *pross_waw_had = NULL;
    // FILE                *src_file   = NULL;
    // FILE                *dest_file  = NULL;
    RiffChunk_t         *RiffChunk  = NULL;
    FmtChunk_t          *FmtChunk   = NULL;
    DataChunk_t         *DataChunk  = NULL;
    void                *audio      = NULL;
    size_t              sizeNms     = 0;
    size_t              tmpSize     = 0;

    if((src_file = fhand_parse_wav_file_fptr(pross_waw->src_f_path, RiffChunk, FmtChunk, DataChunk)) == NULL){
        fprintf(stderr,RED"Error: "BOLDWHITE"File unparse. Reject\n"RESET);
        return -1;
    }

    FmtChunk->chunkSize =  sizeof(FmtChunk_t) - 8;
    sizeNms = ((FmtChunk->sampleRate/1000)*pross_waw->size_ms)*FmtChunk->blockAlign;

    tmpSize = (size_t)DataChunk->chunkSize;
    if ((tmpSize % sizeNms) == 0){
        DataChunk->chunkSize = DataChunk->chunkSize;
    } else {
        DataChunk->chunkSize = ((tmpSize / sizeNms) + 1) * sizeNms;
    }
    RiffChunk->chunkSize = sizeof(RiffChunk_t) - 8 + sizeof(RiffChunk_t) + sizeof(DataChunk_t) + DataChunk->chunkSize;


    if((dest_file = fhand_newav(pross_waw->dest_f_path, RiffChunk, FmtChunk, DataChunk)) == NULL){
        fprintf(stderr, RED"Error: "BOLDWHITE"File unparse. Reject\n"RESET);
        free_chunk_hdr(RiffChunk, FmtChunk, DataChunk);
        fclose(src_file);
        return -1;
    }

    free_chunk_hdr(RiffChunk, FmtChunk, DataChunk);

    process(pross_waw_had);

    fclose(src_file);
    fclose(dest_file);

    return 0;

}

int process(pross_waw_hand_t *pross_waw_had){
    void *audio = malloc(pross_waw_had->sizeNms);
    if (audio == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    memset(audio, 0, pross_waw_had->sizeNms);

    while (pross_waw_had->allAudiosize > pross_waw_had->sizeNms){
        fread(audio, 1, pross_waw_had->sizeNms, pross_waw_had->src_file);
        pross_waw_had->effect_process(pross_waw_had->coeffs, pross_waw_had->states, pross_waw_had->samples_count);
        fwrite(audio, 1,  pross_waw_had->sizeNms, pross_waw_had->dest_file);
        pross_waw_had->allAudiosize -= pross_waw_had->sizeNms;
    }

    if(pross_waw_had->allAudiosize != 0){
        memset(audio, 0, pross_waw_had->sizeNms);
        fread(audio, 1, pross_waw_had->allAudiosize, pross_waw_had->src_file);
        fwrite(audio, 1,  pross_waw_had->sizeNms, pross_waw_had->dest_file);
    }
}
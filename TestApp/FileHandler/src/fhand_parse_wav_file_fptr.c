#include "file_handler.h"

/*****************************************************************************************************/

FILE *fhand_parse_wav_file_fptr(const char *path, wav_hdr_t *hdr){

    FILE *file = NULL;

    file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        return file;
    }
    //Alloc mem for hdr chunk
    alloc_mem_chunk_hdr(hdr);

    while(hdr->RiffChunk->chunkId != RIFF_CHUNK_ID && !feof(file)){
        fread (&(hdr->RiffChunk->chunkId), sizeof(hdr->RiffChunk->chunkId), 1, file);
    }
    if(hdr->RiffChunk->chunkId == RIFF_CHUNK_ID){
        //fread (&(hdr->RiffChunk->chunkId), (sizeof(RiffChunk_t) - 4), 1, file);
        fread(&hdr->RiffChunk->chunkSize, sizeof(uint32_t), 1, file);
        fread(hdr->RiffChunk->format, sizeof(char), 4, file);
    }else{
        fprintf(stderr,RED"Error: "BOLDWHITE"'RIFF' chunk not found\n"RESET);
        free_chunk_hdr(hdr);
        fclose(file);
        return NULL;
    }

    while(hdr->FmtChunk->chunkId != fmt_CHUNK_ID && !feof(file)){
        fread (&(hdr->FmtChunk->chunkId), sizeof(uint32_t), 1, file);
    }

    if(hdr->FmtChunk->chunkId == fmt_CHUNK_ID){
        //fread (&(hdr->RffChunk->chunkId),  1, (sizeof(FmtChunk_t) - 4), file);
        fread(&hdr->FmtChunk->chunkSize, sizeof(uint32_t), 1, file);
        fread(&hdr->FmtChunk->audioFormat, sizeof(uint16_t), 1, file);
        fread(&hdr->FmtChunk->numChannels, sizeof(uint16_t), 1, file);
        fread(&hdr->FmtChunk->sampleRate, sizeof(uint32_t), 1, file);
        fread(&hdr->FmtChunk->byteRate, sizeof(uint32_t), 1, file);
        fread(&hdr->FmtChunk->blockAlign, sizeof(uint16_t), 1, file);
        fread(&hdr->FmtChunk->bitsPerSample, sizeof(uint16_t), 1, file);
        fseek(file, (hdr->FmtChunk->chunkSize - 16), SEEK_CUR);
    }else{
        fprintf(stderr,RED"Error: "BOLDWHITE"'Fmt' chunk not found\n"RESET);
        free_chunk_hdr(hdr);
        fclose(file);
        return NULL;
    }


    while(hdr->DataChunk->chunkId != data_CHUNK_ID && !feof(file)){
        fread(&(hdr->DataChunk->chunkId), sizeof(uint32_t), 1, file);
        fread(&hdr->DataChunk->chunkSize, sizeof(uint32_t), 1, file);
        if(hdr->DataChunk->chunkId != data_CHUNK_ID){
            fseek(file, (hdr->DataChunk->chunkSize), SEEK_CUR);
        }
    }

    if(hdr->DataChunk->chunkId == data_CHUNK_ID){

    }else{
        fprintf(stderr,RED"Error: "BOLDWHITE"'data' chunk not found\n"RESET);
        free_chunk_hdr(hdr);
        fclose(file);
        return NULL;
    }
    
    return file;
}

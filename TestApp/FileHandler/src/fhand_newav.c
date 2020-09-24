#include "file_handler.h"

FILE *fhand_newav(const char *path, wav_hdr_t *hdr){
    FILE *file;

    file = fopen(path, "wb");
    if (file == NULL) {
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        return file;
    }

    fwrite(&hdr->RiffChunk->chunkId, sizeof(uint32_t), 1, file);
    fwrite(&hdr->RiffChunk->chunkSize, sizeof(uint32_t), 1, file);
    fwrite(hdr->RiffChunk->format, sizeof(char), 4, file);

    fwrite(&hdr->FmtChunk->chunkId, sizeof(uint32_t), 1, file);
    fwrite(&hdr->FmtChunk->chunkSize, sizeof(uint32_t), 1, file);
    fwrite(&hdr->FmtChunk->audioFormat, sizeof(uint16_t), 1, file);
    fwrite(&hdr->FmtChunk->numChannels, sizeof(uint16_t), 1, file);
    fwrite(&hdr->FmtChunk->sampleRate, sizeof(uint32_t), 1, file);
    fwrite(&hdr->FmtChunk->byteRate, sizeof(uint32_t), 1, file);
    fwrite(&hdr->FmtChunk->blockAlign, sizeof(uint16_t), 1, file);
    fwrite(&hdr->FmtChunk->bitsPerSample, sizeof(uint16_t), 1, file);

    fwrite(&(hdr->DataChunk->chunkId), sizeof(uint32_t), 1, file);
    fwrite(&hdr->DataChunk->chunkSize, sizeof(uint32_t), 1, file);

    return file;
}

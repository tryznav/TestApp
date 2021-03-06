#include "test_app.h"

/*****************************************************************************************************/

void alloc_mem_chunk_hdr(wav_hdr_t *hdr){

    hdr->RiffChunk = (RiffChunk_t *)malloc(sizeof(RiffChunk_t));
    if (hdr->RiffChunk == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    memset(hdr->RiffChunk, 0, sizeof(RiffChunk_t));

    hdr->FmtChunk = (FmtChunk_t *)malloc(sizeof(FmtChunk_t));
    if (hdr->FmtChunk == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    memset(hdr->FmtChunk, 0, sizeof(FmtChunk_t));

    hdr->DataChunk = (DataChunk_t *)malloc(sizeof(DataChunk_t));
    if (hdr->DataChunk == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    memset(hdr->DataChunk, 0, sizeof(DataChunk_t));
}

//******************************************************************************************************

long fhand_filelength(FILE *file){
    long length = 0;

    if (!file) {
        fprintf(stderr,RED"Error: "BOLDWHITE"Wrong file pointer. Rejected\n"RESET);
        return 0;
    }

    fseek (file, 0, SEEK_END);
    length = ftell (file);
    fseek (file, 0, SEEK_SET);

    return length;   
}

//******************************************************************************************************

void *fhand_file_to_mem(
    const char *path,
    long *length){
    FILE *file = NULL;
    void *res_buff = NULL;

    file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    *length = fhand_filelength(file);
    if(*length == 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"File is empty.\n"RESET);
        return res_buff;
    }

    res_buff = malloc (*length);
    if (res_buff)
    {
        fread (res_buff, 1, *length, file);
    }
    fclose (file);
    return res_buff;
}

//******************************************************************************************************

void *fhand_find_chunk_id(char *ptr, long length, uint32_t Id){
    void *  res = NULL;

    for (int i = 0; i < (length - 4); i++){
        if(*((uint32_t *)(ptr + i)) == Id ){
            return (void *)(ptr + i);
        }
    }
    
    return  NULL;//res;
}

//******************************************************************************************************

void fhand_create_wav_file(const char *path, RiffChunk_t *RiffChunk, FmtChunk_t *FmtChunk, DataChunk_t *DataChunk, void *dat){
    FILE *fp = fopen(path, "w+");

    fwrite(RiffChunk, 1, sizeof(RiffChunk_t), fp);
    fwrite(FmtChunk, 1, sizeof(FmtChunk_t), fp);
    fwrite(DataChunk, 1, (sizeof(FmtChunk_t)), fp);
    fwrite(dat, 1, (DataChunk->chunkSize), fp);
    fclose(fp);
}

//******************************************************************************************************

void fhand_parse_wav_file(const char *path, RiffChunk_t *RiffChunk, FmtChunk_t *FmtChunk, DataChunk_t *DataChunk, void *dat, void *file_buff){
    long length = 0;
    file_buff = fhand_file_to_mem(path, &length);

    FmtChunk = fhand_find_chunk_id(file_buff, length, fmt_CHUNK_ID);         //RIFF Chunk
    if (FmtChunk == NULL) {
        fprintf(stderr,RED"Error: "BOLDWHITE"'Fmt' chunk not found\n"RESET);
    }
    RiffChunk = fhand_find_chunk_id(file_buff, length, RIFF_CHUNK_ID);        //RIFF Chunk
    if (RiffChunk == NULL) {
        fprintf(stderr,RED"Error: "BOLDWHITE"'RIFF' chunk not found\n"RESET);
    }
    DataChunk = fhand_find_chunk_id(file_buff, length, data_CHUNK_ID);
    if (DataChunk == NULL) {
        fprintf(stderr,RED"Error: "BOLDWHITE"'data' chunk not found\n"RESET);
    }
    dat = DataChunk;
    dat = (char *)dat + sizeof(DataChunk_t);

    fhand_print_chunk(RiffChunk, FmtChunk, DataChunk);

}

//******************************************************************************************************

void fhand_print_chunk(RiffChunk_t *RiffChunk, FmtChunk_t *FmtChunk, DataChunk_t *DataChunkh){
    char Id[4];
    memcpy(Id, &(RiffChunk->chunkId), 4);
    printf("\n#########################################\n");
    printf("ChunkID\t\t\t\t%c%c%c%c\n", Id[0], Id[1], Id[2], Id[3]);
    printf("Chunk Data Size\t\t\t%d\n", RiffChunk->chunkSize);
    printf("RIFF Type\t\t\t%c%c%c%c\n", RiffChunk->format[0], RiffChunk->format[1], RiffChunk->format[2], RiffChunk->format[3]);
    printf("+++++++++++++++++++++++++++++++++++++++++\n");
    memcpy(Id, &(RiffChunk->chunkId), 4);
    printf("ChunkID\t\t\t\t%c%c%c%c\n", Id[0], Id[1], Id[2], Id[3]);
    printf("Chunk Data Size\t\t\t%d\n", FmtChunk->chunkSize);
    printf("AudioFormat\t\t\t%d\n",     FmtChunk->audioFormat);
    printf("Number of channels\t\t%d\n", FmtChunk->numChannels);
    printf("Sample rate\t\t\t%d\n",     FmtChunk->sampleRate);
    printf("Average bytes per second\t%u\n", FmtChunk->byteRate);
    printf("Block align\t\t\t%d\n",     FmtChunk->blockAlign);
    printf("Significant bits per sample\t%d\n", FmtChunk->bitsPerSample);
    printf("+++++++++++++++++++++++++++++++++++++++++\n");
    memcpy(Id, &(DataChunkh->chunkId), 4);
    printf("ChunkID\t\t\t\t%c%c%c%c\n", Id[0], Id[1], Id[2], Id[3]);
    printf("Chunk Data Size\t\t\t%d\n", DataChunkh->chunkSize);
    printf("#########################################\n\n");
}

/*************************************************************************************************/

int fhand_resize_wav(uint32_t size_ms, const char *src_f_path, const char *dest_f_path){
    long length = 0;
    uint32_t new_data_size = 0;
    void *tmp_data = NULL;

    FmtChunk_t      *FmtChunk   = NULL;
    RiffChunk_t     *RiffChunk  = NULL;
    DataChunk_t     *DataChunk  = NULL;
    void            *dat        = NULL;
    void            *file_buff  = NULL;

    fhand_parse_wav_file(src_f_path, RiffChunk, FmtChunk, DataChunk, dat, file_buff);

    FmtChunk->chunkSize =  sizeof(FmtChunk_t) - 8;
    new_data_size = ((FmtChunk->sampleRate/1000)*size_ms)*FmtChunk->blockAlign;
    RiffChunk->chunkSize = sizeof(RiffChunk_t) - 8 + sizeof(RiffChunk_t) + sizeof(DataChunk_t) + new_data_size;

    if(DataChunk->chunkSize < new_data_size){
        DataChunk_t *DataTmp = (DataChunk_t *)malloc(new_data_size);
        memset(DataTmp , 0, new_data_size);
        memcpy(DataTmp , dat, DataChunk->chunkSize);
        // tmp_data = 
        fhand_create_wav_file(dest_f_path, RiffChunk, FmtChunk, DataChunk, DataTmp);
        free(DataTmp);
    }else{
        DataChunk->chunkSize = new_data_size;
        fhand_create_wav_file(dest_f_path, RiffChunk, FmtChunk, DataChunk, dat);
    }
    free(file_buff);
    return 0;
}

/*****************************************************************************************************/

int32_t fhand_newhdr_f(wav_hdr_t *hdr, uint32_t sample_rate, uint32_t length_sample, uint16_t audioFormat){
    
    alloc_mem_chunk_hdr(hdr);

    hdr->RiffChunk->chunkId = RIFF_CHUNK_ID;
    memcpy(hdr->RiffChunk->format,"WAVE",4);

    hdr->FmtChunk->chunkId = fmt_CHUNK_ID;
    hdr->FmtChunk->chunkSize = sizeof(FmtChunk_t);
    hdr->FmtChunk->audioFormat = audioFormat;
    hdr->FmtChunk->numChannels = STEREO_DATA;
    hdr->FmtChunk->sampleRate = sample_rate;
    hdr->FmtChunk->byteRate = hdr->FmtChunk->numChannels * hdr->FmtChunk->sampleRate * sizeof(float);
    hdr->FmtChunk->blockAlign = sizeof(float) * hdr->FmtChunk->numChannels;
    hdr->FmtChunk->bitsPerSample = sizeof(float) * 8;
    
    hdr->DataChunk->chunkId = data_CHUNK_ID;
    hdr->DataChunk->chunkSize = hdr->FmtChunk->blockAlign * length_sample;
    
    hdr->RiffChunk->chunkSize = sizeof(RiffChunk_t) - 8 + sizeof(FmtChunk_t) + sizeof(DataChunk_t) + hdr->DataChunk->chunkSize;

    return 0;
}

/*****************************************************************************************************/

void free_chunk_hdr(wav_hdr_t *hdr){
    free(hdr->RiffChunk);
    hdr->RiffChunk = NULL;
    free(hdr->FmtChunk);
    hdr->RiffChunk = NULL;
    free(hdr->DataChunk);
    hdr->RiffChunk = NULL;
}

/*****************************************************************************************************/


#ifndef __FILE_HANDLER_H__
#define __FILE_HANDLER_H__

//headers for file handling
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

#include "colors.h"

#define     RIFF_CHUNK_ID       'FFIR'  //0x52494646
#define     fmt_CHUNK_ID        ' tmf'  //0x666D7420
#define     data_CHUNK_ID       'atad'  //0x64617461

#define     PMC                 1
#define     IEEE_754            3

#define     STEREO_DATA         2


#pragma pack(push,1)
typedef struct RiffChunk_s{
    uint32_t        chunkId;
    uint32_t	    chunkSize;          // Это оставшийся размер цепочки, начиная с этой позиции.
    char            format[4];          // Содержит символы "WAVE"// (0x57415645 в big-endian представлении)
}RiffChunk_t;

typedef struct FmtChunk_s{
    uint32_t        chunkId;
    uint32_t        chunkSize;              // Это оставшийся размер подцепочки, начиная с этой позиции.
    uint16_t        audioFormat;            // Аудио формат
    uint16_t        numChannels;            // Количество каналов. Моно = 1, Стерео = 2 и т.д.
    uint32_t        sampleRate;             // Частота дискретизации.
    uint32_t        byteRate;
    uint16_t        blockAlign;              // Количество байт для одного сэмпла, включая все каналы.
    uint16_t        bitsPerSample;           // Так называемая "глубиная" или точность звучания.
}FmtChunk_t;

typedef struct DataChunk_s
{
    uint32_t        chunkId;
    uint32_t        chunkSize;
    //...
    //Data
}DataChunk_t;

typedef struct wav_hdr_s{
    RiffChunk_t     *RiffChunk;
    FmtChunk_t      *FmtChunk;
    DataChunk_t     *DataChunk;
}wav_hdr_t;

#pragma pack(pop)
typedef struct pross_waw_s{
    const char      *src_f_path;
    const char      *dest_f_path;
    size_t size_ms;
    //process
    int32_t         (*effect_process_get_sizes)(size_t*);
    int32_t         (*effect_reset)(void const*, void*);
    int32_t         (*effect_process)(void const*, void*, void*, size_t);
    //controll
    // int32_t         (*effect_control_get_sizes)(size_t*, size_t*);
    void            *params;
    void            *coeffs;
    int32_t         (*effect_control_initialize)(void*, void*, uint32_t);
    int32_t         (*effect_set_parameter)(void*, int32_t, float);
    int32_t         (*effect_update_coeffs)(void const*, void*);
}pross_waw_t;

typedef struct process_waw_hand_s{
    size_t      allAudiosize;
    size_t      sizeNms;
    FILE        *src_file;
    FILE        *dest_file;
    int32_t     (*effect_process)(void const*, void*, void*, size_t);
    void const* coeffs;
    void*       states;
    uint16_t    audioFormat;            // Аудио формат
    size_t      samples_count;
    
}process_waw_hand_t;

/****************************************************************************************************************
* fhand_file_to_mem() : reading data from a file into allocated memory
*
* @path:        Path to file
* @length:
*
* Return:       The pointer to the memory in HEAP which stores the data from file or NULL in error case.
*****************************************************************************************************************/

void *fhand_file_to_mem(
    const char *path,
    long *length);

/****************************************************************************************************************
* fhand_file_to_mem() : calculate length of file in byte
*
* @file:        Pointer to open file
*
* Return:       File length, 0 in error case or if file empty.
*****************************************************************************************************************/

long fhand_filelength(
    FILE *file);


void *fhand_find_chunk_id(char *ptr, long length, uint32_t Id);

void fhand_create_wav_file(const char *path, RiffChunk_t *RiffChunk, FmtChunk_t *FmtChunk, DataChunk_t *DataChunk, void *dat);

void *fhand_find_chunk_id(char *ptr, long length, uint32_t Id);

void fhand_parse_wav_file(const char *path, RiffChunk_t *RiffChunk, FmtChunk_t *FmtChunk, DataChunk_t *DataChunk, void *dat, void *file_buff);

void fhand_print_chunk(RiffChunk_t *RiffChunk, FmtChunk_t *FmtChunk, DataChunk_t *DataChunkh);

int fhand_resize_wav(uint32_t size_ms, const char *src_f_path, const char *dest_f_path);

FILE *fhand_newav(const char *path, wav_hdr_t *hdr);

FILE *fhand_parse_wav_file_fptr(const char *path, wav_hdr_t *hdr);

int fhand_wavdup(const char *src_f_path, const char *dest_f_path,size_t size_ms);

/******************************************************************************************************/
//help func
void alloc_mem_chunk_hdr(wav_hdr_t *hdr);
void free_chunk_hdr(wav_hdr_t *hdr);
int32_t fhand_wav_process(pross_waw_t *pross_waw);

#endif
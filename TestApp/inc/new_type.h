#ifndef __NEW_TYPE_H__
#define __NEW_TYPE_H__

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

//WAV FILE CHUNCS
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

typedef struct gen_sig_s{
    uint32_t        sample_rate;
    uint32_t        length_sample;
    void            *states;
    void            *params;
    void *          (*tsig_sig_init_states)(uint32_t , uint32_t , void const *, uint16_t);                  //void *tsig_delta_init_states    (uint32_t sample_rate, uint32_t length_sample, void const *params, uint16_t audioFormat)
    int32_t         (*tsig_gen_sig_st)(uint32_t , void* , void *);                                          //int32_t tsig_gen_delta_st   (uint32_t length_sample, void* states, void *audio)
}gen_sig_t;

typedef struct efect_s{
    void            *coeffs;
    void            *states;
    void            *params;

    uint16_t        audioFormat;            // Аудио формат
    size_t          samples_count;


    int32_t         (*effect_process_get_sizes)(size_t*);
    int32_t         (*effect_reset)(void const*, void*);
    int32_t         (*effect_process)(void const*, void*, void*, size_t);

    int32_t         (*effect_control_get_sizes)(size_t* , size_t* );  
    int32_t         (*effect_control_initialize)(void*, void*, uint32_t);
    int32_t         (*effect_set_parameter)(void*, int32_t, float);
    int32_t         (*effect_update_coeffs)(void const*, void*);

}effect_t;


typedef struct buff_s{
    size_t  size_ms;
    size_t  size_byte;
    size_t  samples;
}size_my_t;

typedef struct pross_waw_s{
    const char      *src_f_path;
    const char      *dest_f_path;
    FILE            *dest_file;
    FILE            *src_file;
    size_my_t        audio;
    size_my_t        buff;
    //process
    effect_t         *efect;
    gen_sig_t       *gen_sig;
}pross_waw_t;


// typedef struct process_waw_hand_s{

//     int32_t         (*effect_process)(void const*, void*, void*, size_t);
//     gen_sig_t       *gen_sig;
// }process_waw_hand_t;



// /****************************************************************************************************************
// * fhand_file_to_mem() : reading data from a file into allocated memory
// *
// * @path:        Path to file
// * @length:
// *
// * Return:       The pointer to the memory in HEAP which stores the data from file or NULL in error case.
// *****************************************************************************************************************/

// void *fhand_file_to_mem(
//     const char *path,
//     long *length);

// /****************************************************************************************************************
// * fhand_file_to_mem() : calculate length of file in byte
// *
// * @file:        Pointer to open file
// *
// * Return:       File length, 0 in error case or if file empty.
// *****************************************************************************************************************/


#endif

#include "test_app.h"

void app_gain(char *input_f_path, char *output_f_path, float gain_dB){
    RiffChunk_t     *RiffChunk = NULL;
    FmtChunk_t      *FmtChunk = NULL;
    DataChunk_t     *DataChunk = NULL; 
    void            *file_buff = NULL;
    void            *audio_for_effect = NULL;
    size_t          samples_count = 0;
    void            *dat;

    int32_t         Res;

    //efect controll 
    size_t          params_bytes;
    size_t          coeffs_bytes;
    void            *params = NULL;
    void            *coeffs = NULL;

    //efect process
    size_t          states_bytes;
    void*           states;

    fhand_parse_wav_file(input_f_path, FmtChunk, RiffChunk, DataChunk, dat, file_buff);
    if(FmtChunk->numChannels != 2) {
        fprintf(stderr,RED"Error: Wrong audio format. "BOLDWHITE"App support stereo data.\n"RESET);
        exit(EXIT_FAILURE);
    }
    if(FmtChunk->audioFormat != 1 || FmtChunk->audioFormat != 3){
        fprintf(stderr,RED"Error: Wrong audio format. "BOLDWHITE"App support PMC and IEEE 754 formats.\n"RESET);
        exit(EXIT_FAILURE);
    }
    samples_count = DataChunk->chunkSize/FmtChunk->blockAlign;
    audio_for_effect = conv_aud_for_eff(FmtChunk, RiffChunk, DataChunk, samples_count);

    if((Res = effect_control_get_sizes(&params_bytes, &coeffs_bytes)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_control_get_sizes()\n"RESET);
        exit(EXIT_FAILURE);
    }
    
    params = malloc(params_bytes);
    coeffs = malloc(coeffs_bytes);

    if((Res = effect_control_initialize(params, coeffs, FmtChunk->sampleRate)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_control_initialize()\n"RESET);
        exit(EXIT_FAILURE);
    }

    if((Res = effect_set_parameter(params, 1, gain_dB)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_set_parameter()\n"RESET);
        exit(EXIT_FAILURE);
    }
    if((Res = effect_update_coeffs(params, coeffs)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_control_get_sizes()\n"RESET);
        exit(EXIT_FAILURE);
    }

    if((Res = effect_process_get_sizes(&states_bytes)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_control_get_sizes()\n"RESET);
        exit(EXIT_FAILURE);
    }

    states = malloc(states_bytes);

    if((Res = effect_process(coeffs, states, audio_for_effect, samples_count) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_control_get_sizes()\n"RESET);
        exit(EXIT_FAILURE);
    }

    fhand_create_wav_file(input_f_path, output_f_path, audio_for_effect);

    *states_bytes = sizeof(float);

    return 0;
}

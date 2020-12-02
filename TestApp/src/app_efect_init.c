#include "test_app.h"
#include "cJSON.h"

int32_t app_efect_init(pross_waw_t *pr, uint32_t sample_rate){
    size_t      params_bytes = 0;
    size_t      coeffs_bytes = 0;
    size_t      states_bytes = 0;
    void*       params = NULL;
    int32_t     Res;

    effect_t*   effect = (effect_t *)malloc(sizeof(effect_t));

    if (effect == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    effect->coeffs = NULL;
    effect->states = NULL;

    if((Res = effect_control_get_sizes(&params_bytes, &coeffs_bytes)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_control_get_sizes()\n"RESET);
        exit(EXIT_FAILURE);
    }

    if((Res = effect_process_get_sizes(&states_bytes)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_update_coeffs\n"RESET);
        // exit(EXIT_FAILURE);
    }

    params = malloc(params_bytes);
    if (params == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    effect->coeffs = malloc(coeffs_bytes);
    if (effect->coeffs == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    effect->states = malloc(states_bytes);
    if (effect->states == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
    }
    if((Res = effect_control_initialize(params, effect->coeffs, sample_rate)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_update_coeffs\n"RESET);
        // exit(EXIT_FAILURE);
    }



    if((Res = effect_update_coeffs(params, effect->coeffs)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_update_coeffs\n"RESET);
        // exit(EXIT_FAILURE);
    }
    
    if((Res = effect_reset(effect->coeffs, effect->states)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_update_coeffs\n"RESET);
    }

     
    free(params);
    printf("here1\n");
    pr->effect = effect;
    // free(pr->effect->states);
    // free(pr->effect->coeffs);

    return 0;
}

static void effect_set_preset(void *params){
    int Res;
    
    FILE * js = fopen("D:/TestApp/TestApp/Effect/preset.json", "r");
    
    fseek(js, 0, SEEK_END);
    size_t size = ftell(js);
    fseek(js, 0, SEEK_SET);

    char * buffer = malloc(size);
    memset(buffer, 0, size);
    fread(buffer, size, 1, js);

    const cJSON *Effects = NULL;
    const cJSON *Sub_Effects = NULL;
    const cJSON *bands = NULL;

    cJSON *json = cJSON_Parse(buffer);

    // bands = cJSON_GetObjectItemCaseSensitive(json, "eq_params");
    cJSON_ArrayForEach(Effects, json)
    {
        cJSON *Effect_ID    = cJSON_GetObjectItemCaseSensitive(Effects, "Effect ID");
        cJSON *Effect_Name = cJSON_GetObjectItemCaseSensitive(Effects, "Effect Name");
        printf("%s %d\n", Effect_Name->valuestring, Effect_ID->valueint);

    }
    // if((Res = effect_set_parameter(params, PRM_GAIN_dB_ID,value)) != 0){
    //     fprintf(stderr,RED"Error: "BOLDWHITE"effect_set_parameter(PRM_GAIN_dB_ID)\n"RESET);
    //     // exit(EXIT_FAILURE);
    // }
}
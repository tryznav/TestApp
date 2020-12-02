#include "test_app.h"
#include "cJSON.h"
char *create_monitor(void);
static void effect_set_preset(void *params);

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

    effect_set_preset(params);

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
    // int Res;
    
    FILE * js = fopen("D:/TestApp/TestApp/Effect/preset_2.json", "rb");

    //FILE * wri = fopen("D:/TestApp/TestApp/Effect/preset_3.json", "wb");
    //char *s = create_monitor();
    //fprintf(wri,"%s", s);
    //free(s);
    //fclose(wri);
    
    fseek(js, 0, SEEK_END);
    size_t size = ftell(js);
    fseek(js, 0, SEEK_SET);

    char * buffer = malloc(size);
    memset(buffer, 0, size);
    fread(buffer, size, 1, js);

    const cJSON *Effects = NULL;
    const cJSON *Sub_Effects = NULL;
    // const cJSON *bands = NULL;
    cJSON *json = cJSON_Parse(buffer);

    cJSON *chain = cJSON_GetObjectItemCaseSensitive(json, "chain");
    cJSON *chain_name = cJSON_GetObjectItemCaseSensitive(json, "name");
    printf("%s dmkasmdklamkl \n", chain_name->valuestring);
    cJSON_ArrayForEach(Effects, chain)
    {
        cJSON *Effect_ID    = cJSON_GetObjectItemCaseSensitive(Effects, "Effect ID");
        cJSON *Effect_Name  = cJSON_GetObjectItemCaseSensitive(Effects, "Effect Name");
        printf("%s %d\n", Effect_Name->valuestring, Effect_ID->valueint);

        cJSON *Sub_Effects  = cJSON_GetObjectItemCaseSensitive(Effects, "Sub Effects");
        cJSON *Sub_Effect   = NULL;
        cJSON_ArrayForEach(Sub_Effect, Sub_Effects)
        {
            cJSON *Sub_Effect_ID   = cJSON_GetObjectItemCaseSensitive(Sub_Effect, "Sub Effect ID");
            cJSON *Sub_Effect_Name = cJSON_GetObjectItemCaseSensitive(Sub_Effect, "Effect Name");
            printf("%s %d\n", Sub_Effect_Name->valuestring, Sub_Effect_ID->valueint);
            cJSON *Parameters = cJSON_GetObjectItemCaseSensitive(Sub_Effect, "Parameters");
            cJSON *Parameter = NULL;
            cJSON_ArrayForEach(Parameter, Parameters)
            {   
                cJSON *Parameter_ID    = cJSON_GetObjectItemCaseSensitive(Parameter, "Parameter ID");
                cJSON *Parameter_Name = cJSON_GetObjectItemCaseSensitive(Parameter, "Parameter Name");
                printf("%s\t%d\n", Parameter_Name->valuestring, Parameter_ID->valueint);
            }
        }
    }
    fclose(js);
    // if((Res = effect_set_parameter(params, PRM_GAIN_dB_ID,value)) != 0){
    //     fprintf(stderr,RED"Error: "BOLDWHITE"effect_set_parameter(PRM_GAIN_dB_ID)\n"RESET);
    //     // exit(EXIT_FAILURE);
    // }
}

char *create_monitor(void)
{
    const unsigned int resolution_numbers[3][2] = {
        {1280, 720},
        {1920, 1080},
        {3840, 2160}
    };
    char *string = NULL;
    cJSON *name = NULL;
    cJSON *resolutions = NULL;
    cJSON *resolution = NULL;
    cJSON *width = NULL;
    cJSON *height = NULL;
    size_t index = 0;

    cJSON *monitor = cJSON_CreateObject();
    if (monitor == NULL)
    {
        goto end;
    }

    name = cJSON_CreateString("Effects");
    if (name == NULL)
    {
        goto end;
    }
    /* after creation was successful, immediately add it to the monitor,
     * thereby transferring ownership of the pointer to it */
    cJSON_AddItemToObject(monitor, "name", name);

    resolutions = cJSON_CreateArray();
    if (resolutions == NULL)
    {
        goto end;
    }
    cJSON_AddItemToObject(monitor, "resolutions", resolutions);

    for (index = 0; index < (sizeof(resolution_numbers) / (2 * sizeof(int))); ++index)
    {
        resolution = cJSON_CreateObject();
        if (resolution == NULL)
        {
            goto end;
        }
        cJSON_AddItemToObject(monitor, "resolutions", resolutions);
        cJSON_AddItemToArray(resolutions, resolution);

        width = cJSON_CreateNumber(resolution_numbers[index][0]);
        if (width == NULL)
        {
            goto end;
        }
        cJSON_AddItemToObject(resolution, "width", width);

        height = cJSON_CreateNumber(resolution_numbers[index][1]);
        if (height == NULL)
        {
            goto end;
        }
        cJSON_AddItemToObject(resolution, "height", height);
    }

    string = cJSON_Print(monitor);
    if (string == NULL)
    {
        fprintf(stderr, "Failed to print monitor.\n");
    }

end:
    cJSON_Delete(monitor);
    return string;
}
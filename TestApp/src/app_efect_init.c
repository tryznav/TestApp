#include "test_app.h"
#include <json.h>

static int32_t apf_settings_write();

#define PRESET_FILE "preset.json"

static int32_t effect_control(effect_t *effect, wav_hdr_t  *hdr, effect_task_t *effect_task);
// static int32_t effect_states_init(pross_waw_t *pr);

int32_t app_efect_init(pross_waw_t *pr, wav_hdr_t  *hdr, effect_task_t *effect_task){

    effect_t*    efect = (effect_t *)malloc(sizeof(effect_t));
    if (efect == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    efect->coeffs = NULL;
    efect->params = NULL;
    efect->states = NULL;

    switch (hdr->FmtChunk->audioFormat)
    {
    case PCM:
        printf(BOLDCYAN"PCM\n"RESET);
        switch (effect_task->effect_type)
        {
        // case    EFFECT_ID_GAIN: 
        //     pr->efect->effect_control_initialize = &effect_control_initialize;
        //     pr->efect->effect_process            = &effect_process;
        //     pr->efect->effect_process_get_sizes  = &effect_process_get_sizes;
        //     pr->efect->effect_reset              = &effect_reset;
        //     pr->efect->effect_set_parameter      = &effect_set_parameter;
        //     pr->efect->effect_update_coeffs      = &effect_update_coeffs;
        //     break;
        case    EFFECT_ID_FIR:
            //controll
            efect->effect_control_get_sizes  = &fir_fxd_control_get_sizes;
            efect->effect_control_initialize = &fir_fxd_control_initialize;
            efect->effect_set_parameter      = &fir_fxd_set_parameter;
            efect->effect_update_coeffs      = &fir_fxd_update_coeffs;
            //process
            efect->effect_process            = &fir_fxd_process;
            efect->effect_process_get_sizes  = &fir_fxd_process_get_sizes;
            efect->effect_reset              = &fir_fxd_reset;
            break;
        case    EFFECT_ID_IIR:
            //controll
            efect->effect_control_get_sizes  = &iir_fxd_control_get_sizes;
            efect->effect_control_initialize = &iir_fxd_control_initialize;
            efect->effect_set_parameter      = &iir_fxd_set_parameter;
            efect->effect_update_coeffs      = &iir_fxd_update_coeffs;
            //process
            efect->effect_process            = &iir_fxd_process;
            efect->effect_process_get_sizes  = &iir_fxd_process_get_sizes;
            efect->effect_reset              = &iir_fxd_reset;
             break;
        case    EFFECT_ID_APF:
        printf("a");
            //controll
            efect->effect_control_get_sizes  = &apf_fxd_control_get_sizes;
            efect->effect_control_initialize = &apf_fxd_control_initialize;
            efect->effect_set_parameter      = &apf_fxd_set_parameter;
            efect->effect_update_coeffs      = &apf_fxd_update_coeffs;
            //process
            efect->effect_process            = &apf_fxd_process;
            efect->effect_process_get_sizes  = &apf_fxd_process_get_sizes;
            efect->effect_reset              = &apf_fxd_reset;
             break;
        case    EFFECT_ID_CROSSOVER:
            // //controll
            // efect->effect_control_get_sizes  = &cross_fxd_control_get_sizes;
            // efect->effect_control_initialize = &cross_fxd_control_initialize;
            // efect->effect_set_parameter      = &cross_fxd_set_parameter;
            // efect->effect_update_coeffs      = &cross_fxd_update_coeffs;
            // //process
            // efect->effect_process            = &cross_fxd_process;
            // efect->effect_process_get_sizes  = &cross_fxd_process_get_sizes;
            // efect->effect_reset              = &cross_fxd_reset;
            break;
        case    EFFECT_ID_CHAIN:
            //controll
            efect->effect_control_get_sizes  = &chain_fxd_control_get_sizes;
            efect->effect_control_initialize = &chain_fxd_control_initialize;
            efect->effect_set_parameter      = &chain_fxd_set_parameter;
            efect->effect_update_coeffs      = &chain_fxd_update_coeffs;
            //procchain
            efect->effect_process            = &chain_fxd_process;
            efect->effect_process_get_sizes  = &chain_fxd_process_get_sizes;
            efect->effect_reset              = &chain_fxd_reset;
            break;
        default:
            fprintf(stderr,RED" Error: "BOLDWHITE"Not supported format for this effect.\n"RESET);
            return -1;
            break;
        }
        break;
    case IEEE_754:
        printf(BOLDCYAN"IEEE_754\n"RESET);
        switch (effect_task->effect_type)
        {
        case    EFFECT_ID_GAIN:
            //controll
            efect->effect_control_get_sizes  = &effect_control_get_sizes;
            efect->effect_control_initialize = &effect_control_initialize;
            efect->effect_set_parameter      = &effect_set_parameter;
            efect->effect_update_coeffs      = &effect_update_coeffs;
            //process
            efect->effect_process            = &effect_process;
            efect->effect_process_get_sizes  = &effect_process_get_sizes;
            efect->effect_reset              = &effect_reset;
            break;
        case    EFFECT_ID_FIR:
            //controll
            efect->effect_control_get_sizes  = &fir_flt_control_get_sizes;
            efect->effect_control_initialize = &fir_flt_control_initialize;
            efect->effect_set_parameter      = &fir_flt_set_parameter;
            efect->effect_update_coeffs      = &fir_flt_update_coeffs;
            //process
            efect->effect_process            = &fir_flt_process;
            efect->effect_process_get_sizes  = &fir_flt_process_get_sizes;
            efect->effect_reset              = &fir_flt_reset;
            break;
        case    EFFECT_ID_IIR:
            //controll
            efect->effect_control_get_sizes  = &iir_flt_control_get_sizes;
            efect->effect_control_initialize = &iir_flt_control_initialize;
            efect->effect_set_parameter      = &iir_flt_set_parameter;
            efect->effect_update_coeffs      = &iir_flt_update_coeffs;
            //process
            efect->effect_process            = &iir_flt_process;
            efect->effect_process_get_sizes  = &iir_flt_process_get_sizes;
            efect->effect_reset              = &iir_flt_reset;
             break;
        case    EFFECT_ID_CROSSOVER:
            //controll
            // efect->effect_control_get_sizes  = &cross_flt_control_get_sizes;
            // efect->effect_control_initialize = &cross_flt_control_initialize;
            // efect->effect_set_parameter      = &cross_flt_set_parameter;
            // efect->effect_update_coeffs      = &cross_flt_update_coeffs;
            // //process
            // efect->effect_process            = &cross_flt_process;
            // efect->effect_process_get_sizes  = &cross_flt_process_get_sizes;
            // efect->effect_reset              = &cross_flt_reset;
            efect->effect_control_get_sizes  = &chain_flt_control_get_sizes;
            efect->effect_control_initialize = &chain_flt_control_initialize;
            efect->effect_set_parameter      = &chain_flt_set_parameter;
            efect->effect_update_coeffs      = &chain_flt_update_coeffs;
            //procchain
            efect->effect_process            = &chain_flt_process;
            efect->effect_process_get_sizes  = &chain_flt_process_get_sizes;
            efect->effect_reset              = &chain_flt_reset;
            break;
        case    EFFECT_ID_APF:
            //controll
            efect->effect_control_get_sizes  = &apf_flt_control_get_sizes;
            efect->effect_control_initialize = &apf_flt_control_initialize;
            efect->effect_set_parameter      = &apf_flt_set_parameter;
            efect->effect_update_coeffs      = &apf_flt_update_coeffs;
            //process
            efect->effect_process            = &apf_flt_process;
            efect->effect_process_get_sizes  = &apf_flt_process_get_sizes;
            efect->effect_reset              = &apf_flt_reset;
                        //controll

             break;
        case    EFFECT_ID_CHAIN:
            //controll
            efect->effect_control_get_sizes  = &chain_flt_control_get_sizes;
            efect->effect_control_initialize = &chain_flt_control_initialize;
            efect->effect_set_parameter      = &chain_flt_set_parameter;
            efect->effect_update_coeffs      = &chain_flt_update_coeffs;
            //procchain
            efect->effect_process            = &chain_flt_process;
            efect->effect_process_get_sizes  = &chain_flt_process_get_sizes;
            efect->effect_reset              = &chain_flt_reset;
            break;
        default:
            break;
        }
        break;

    default:
        break;
    }

    effect_control(efect, hdr, effect_task);

    pr->efect = efect;

    return 0;
}

static int32_t effect_control(effect_t *effect, wav_hdr_t  *hdr, effect_task_t *effect_task){

    // apf_settings_write();
    size_t params_bytes = 0;
    size_t coeffs_bytes = 0;
    size_t states_bytes = 0;
    int32_t Res = -1;
                // json_object *jobj;
                
                // // json_object *cross_obj;
                // // json_object *cross_prm;

                // json_object *apf_obj;
                // json_object *apf_prm;
                // char string[1000];
                // memset(string, 0, 1000);
                // FILE *file = fopen(PRESET_FILE,"r");

                // if (!file) {
                //     fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
                //     return -1;
                // }
                // while (fgets(string,1000, file)!= NULL);

                // jobj = json_tokener_parse(string);


    if((Res = effect->effect_control_get_sizes(&params_bytes, &coeffs_bytes)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_control_get_sizes()\n"RESET);
        exit(EXIT_FAILURE);
    }
    if((Res = effect->effect_process_get_sizes(&states_bytes)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_update_coeffs\n"RESET);
        // exit(EXIT_FAILURE);
    }
    
    effect->params = malloc(params_bytes);
    if (effect->params == NULL){
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

    if((Res = effect->effect_control_initialize(effect->params, effect->coeffs, hdr->FmtChunk->sampleRate)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_control_initialize\n"RESET);
        // exit(EXIT_FAILURE);
    }
            // json_object_object_get_ex(jobj,"Apf", &apf_obj);
            // // json_object_object_get_ex(jobj,"Crossover", &cross_obj);

            // json_object_object_get_ex(apf_obj,"Cutoff freq", &apf_prm);

            // if((Res = effect->effect_set_parameter(effect->params, PRM_APF_CFREQ, (float)json_object_get_double(apf_prm))) != 0){
            //     fprintf(stderr,RED"Error: "BOLDWHITE"effect_set_parameter(PRM_APF_CFREQ)\n"RESET);
            // }
                
            // json_object_object_get_ex(apf_obj,"Form", &apf_prm);

            // if((Res = effect->effect_set_parameter(effect->params, PRM_APF_FORM, (float)json_object_get_int(apf_prm))) != 0){
            //     fprintf(stderr,RED"Error: "BOLDWHITE"effect_set_parameter(PRM_APF_FORM)\n"RESET);
            // }

            // json_object_object_get_ex(apf_obj,"Order", &apf_prm);

            // if((Res = effect->effect_set_parameter(effect->params, PRM_APF_ORDER, (float)json_object_get_int(apf_prm))) != 0){
            //     fprintf(stderr,RED"Error: "BOLDWHITE"effect_set_parameter(PRM_APF_ORDER)\n"RESET);
            // }



            // json_object_object_get_ex(cross_obj,"f1", &cross_prm);

            // if((Res = effect->effect_set_parameter(effect->params, PRM_CROSSOVER_F0_ID, (float)json_object_get_double(cross_prm) )) != 0){
            //     fprintf(stderr,RED"Error: "BOLDWHITE"effect_set_parameter(PRM_CROSSOVER_F0_ID)\n"RESET);
            // }

            // json_object_object_get_ex(cross_obj,"f2", &cross_prm);

            // if((Res = effect->effect_set_parameter(effect->params, PRM_CROSSOVER_F1_ID, (float)json_object_get_double(cross_prm) )) != 0){
            //     fprintf(stderr,RED"Error: "BOLDWHITE"effect_set_parameter(PRM_CROSSOVER_F1_ID)\n"RESET);
            // }

            // json_object_object_get_ex(cross_obj,"f3", &cross_prm);

            // if((Res = effect->effect_set_parameter(effect->params, PRM_CROSSOVER_F2_ID, (float)json_object_get_double(cross_prm) )) != 0){
            //     fprintf(stderr,RED"Error: "BOLDWHITE"effect_set_parameter(PRM_CROSSOVER_F2_ID)\n"RESET);
            // }

            // json_object_object_get_ex(cross_obj,"g1", &cross_prm);

            // if((Res = effect->effect_set_parameter(effect->params, PRM_CROSSOVER_G1_ID, (float)json_object_get_double(cross_prm) )) != 0){
            //     fprintf(stderr,RED"Error: "BOLDWHITE"effect_set_parameter(PRM_CROSSOVER_G1_ID)\n"RESET);
            // }


            // json_object_object_get_ex(cross_obj,"g2", &cross_prm);

            // if((Res = effect->effect_set_parameter(effect->params, PRM_CROSSOVER_G2_ID, (float)json_object_get_double(cross_prm) )) != 0){
            //     fprintf(stderr,RED"Error: "BOLDWHITE"effect_set_parameter(PRM_CROSSOVER_G2_ID)\n"RESET);
            // }


            // json_object_object_get_ex(cross_obj,"g3", &cross_prm);

            // if((Res = effect->effect_set_parameter(effect->params, PRM_CROSSOVER_G3_ID, (float)json_object_get_double(cross_prm) )) != 0){
            //     fprintf(stderr,RED"Error: "BOLDWHITE"effect_set_parameter(PRM_CROSSOVER_G3_ID)\n"RESET);
            // }

            // json_object_object_get_ex(cross_obj,"g4", &cross_prm);

            // if((Res = effect->effect_set_parameter(effect->params, PRM_CROSSOVER_G4_ID, (float)json_object_get_double(cross_prm) )) != 0){
            //     fprintf(stderr,RED"Error: "BOLDWHITE"effect_set_parameter(PRM_CROSSOVER_G4_ID)\n"RESET);
            // }

            // json_object_object_get_ex(cross_obj,"form1", &cross_prm);

            // if((Res = effect->effect_set_parameter(effect->params, PRM_CROSSOVER_FORM1, (float)json_object_get_int(cross_prm) )) != 0){
            //     fprintf(stderr,RED"Error: "BOLDWHITE"effect_set_parameter( PRM_CROSSOVER_FORM1)\n"RESET);
            // }

            // json_object_object_get_ex(cross_obj,"form2", &cross_prm);

            // if((Res = effect->effect_set_parameter(effect->params, PRM_CROSSOVER_FORM2, (float)json_object_get_int(cross_prm) )) != 0){
            //     fprintf(stderr,RED"Error: "BOLDWHITE"effect_set_parameter(PRM_CROSSOVER_FORM2)\n"RESET);
            // }



            // if((Res = effect->effect_set_parameter(effect->params, PRM_FREQ_START_ID, effect_task->prm.cutoff_freq.sweep.start)) != 0){
            //     fprintf(stderr,RED"Error: "BOLDWHITE"effect_set_parameter(PRM_GAIN_dB_ID)\n"RESET);
            //     // 

            // if((Res = effect->effect_set_parameter(effect->params, PRM_FREQ_START_ID, effect_task->prm.cutoff_freq.sweep.start)) != 0){
            //     fprintf(stderr,RED"Error: "BOLDWHITE"effect_set_parameter(PRM_GAIN_dB_ID)\n"RESET);
            //     // exit(EXIT_FAILURE);
            // }

            // if((Res = effect->effect_set_parameter(effect->params, PRM_FREQ_END_ID, effect_task->prm.cutoff_freq.sweep.end)) != 0){
            //     fprintf(stderr,RED"Error: "BOLDWHITE"effect_set_parameter(PRM_GAIN_dB_ID)\n"RESET);
            //     // exit(EXIT_FAILURE);
            // }

            // if((Res = effect->effect_set_parameter(effect->params, PRM_GAIN_dB_ID, effect_task->prm.gain_dB)) != 0){
            //     fprintf(stderr,RED"Error: "BOLDWHITE"effect_set_parameter(PRM_GAIN_dB_ID)\n"RESET);
            //     // exit(EXIT_FAILURE);
            // }


    if((Res = effect->effect_update_coeffs(effect->params, effect->coeffs)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_update_coeffs\n"RESET);
        // exit(EXIT_FAILURE);
    }

    
    if((Res = effect->effect_reset(effect->coeffs, effect->states)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_update_coeffs\n"RESET);
    }
    // fclose(file);
    

    return 0;
}

static int32_t effect_states_init(effect_t *effect){
    size_t states_bytes = 0;
    int32_t Res = -1;
   
    if((Res = effect->effect_process_get_sizes(&states_bytes)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_update_coeffs\n"RESET);
        // exit(EXIT_FAILURE);
    }
    effect->states = malloc(states_bytes);
    if (effect->states == NULL){
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
    }

    if((Res = effect->effect_reset(effect->coeffs, effect->states)) != 0){
        fprintf(stderr,RED"Error: "BOLDWHITE"effect_update_coeffs\n"RESET);
    }

    return 0;
}

// static int32_t apf_settings_write(){
   
//     FILE *file = fopen(PRESET_FILE, "wb");
//     json_object * jobj = json_object_new_object();
//     json_object * jobj_cross = json_object_new_object();
//     json_object * jobj_apf = json_object_new_object();

//     /*Creating a json integer*/
//     json_object *form = json_object_new_int(1);
//     json_object *order = json_object_new_int(2);

//     /*Creating a json double*/
//     json_object *cutoff_freq = json_object_new_double(200.0);
//     json_object *sample_rate = json_object_new_double(48000);
//     json_object *band_width = json_object_new_double(0.8);

// 	json_object *f1= json_object_new_double(200.0);
// 	json_object *f2= json_object_new_double(300.0);
// 	json_object *f3= json_object_new_double(400.0);
// 	json_object *form1 = json_object_new_int(2);
//     json_object *form2 = json_object_new_int(1);
//     json_object *g1= json_object_new_double(0.0);
// 	json_object *g2= json_object_new_double(0.0);
// 	json_object *g3= json_object_new_double(0.0);
//     json_object *g4= json_object_new_double(0.0);

//         /*Form the json object*/
//     /*Each of these is like a key value pair*/
//     json_object_object_add(jobj_apf,"Sample rate", sample_rate);
//     json_object_object_add(jobj_apf,"Cutoff freq", cutoff_freq);
//     json_object_object_add(jobj_apf,"Band width", band_width);
//     json_object_object_add(jobj_apf,"Form", form);
//     json_object_object_add(jobj_apf,"Order", order);


//     json_object_object_add(jobj_cross,"f1", f1);
//     json_object_object_add(jobj_cross,"f2", f2);    
//     json_object_object_add(jobj_cross,"f3", f3);
//     json_object_object_add(jobj_cross,"Form1", form);
//     json_object_object_add(jobj_cross,"Form2", form);

//     json_object_object_add(jobj_cross,"g1", g1);
//     json_object_object_add(jobj_cross,"g2", g2);
//     json_object_object_add(jobj_cross,"g3", g3);
//     json_object_object_add(jobj_cross,"g4", g4);
//  printf("here\n");
//     // json_object_array_add(jobj, jobj_cross);
//     // json_object_array_add(jobj, jobj_apf);
//     json_object_object_add(jobj,"Crossover", jobj_cross);
//     json_object_object_add(jobj,"Apf", jobj_apf);
//  printf("here1\n");
//     /*Now printing the json object*/
//     const char *str = json_object_to_json_string(jobj);
//     printf("%s\n",str);
//     fprintf(file, "%s", str);
//     fclose(file);
//     return 0;
// }

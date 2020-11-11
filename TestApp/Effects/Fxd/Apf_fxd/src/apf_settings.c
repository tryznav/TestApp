 #include "apf_fxd.h"
 #include <json.h>

#define FILE_NAME       "apf_preset.json"

int32_t apf_settings_write(apf_prm_t *prm){
    FILE *file = fopen(FILE_NAME, "wb");
    json_object * jobj = json_object_new_object();

    /*Creating a json integer*/
    json_object *form = json_object_new_int(prm->form);
    json_object *order = json_object_new_int(prm->order);

    /*Creating a json double*/
    json_object *cutoff_freq = json_object_new_double(prm->freq);
    json_object *sample_rate = json_object_new_double(prm->sample_rate);
    json_object *band_width = json_object_new_double(prm->freq);

    /*Form the json object*/
    /*Each of these is like a key value pair*/
    json_object_object_add(jobj,"Sample rate", sample_rate);
    json_object_object_add(jobj,"Cutoff freq", cutoff_freq);
    json_object_object_add(jobj,"Band width", band_width);
    json_object_object_add(jobj,"Form", form);
    json_object_object_add(jobj,"Order", order);

    /*Now printing the json object*/
    const char *str = json_object_to_json_string(jobj);
    fprintf(file, "%s", str);
    fclose(file);
    return 0;
}

int32_t apf_settings_read(apf_prm_t *prm){
    json_object *jobj;
	json_object *cutoff_freq ;
	json_object *sample_rate;
	json_object *band_width;
	json_object *form;
    json_object *order;
    char string[100];
    FILE *file = fopen(FILE_NAME,"r");

    if (!file) {
        fprintf(stderr,RED"%d: Error: "BOLDWHITE"%s.\n"RESET, errno, strerror(errno));
        return -1;
    }
    while (fgets(string,100, file)!=NULL);

    jobj = json_tokener_parse(string);

    json_object_object_get_ex(jobj,"Sample rate", &sample_rate);
    json_object_object_get_ex(jobj,"Cutoff freq", &cutoff_freq);
    json_object_object_get_ex(jobj,"Band width", &band_width);
    json_object_object_get_ex(jobj,"Form", &form);
    json_object_object_get_ex(jobj,"Order", &order);

    prm->order =  json_object_get_int(order);
    prm->form  =  json_object_get_int(form);

    prm->sample_rate = json_object_get_double(sample_rate);
    prm->freq = json_object_get_double(cutoff_freq);
    // prm->fb = json_object_get_double(band_width);

    fclose(file);
    return 0;
}

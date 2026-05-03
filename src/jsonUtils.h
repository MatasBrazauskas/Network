#pragma once

#include <stdbool.h>

#include <cjson/cJSON.h>

typedef struct {
    char* country;
    char* city;
    char* provider;
    char* host;
    int id;
} Data;

cJSON* readJSON(const char* t_path);
bool validJSON(const cJSON* t_json);
void cleanUpJSON(cJSON *t_json);

Data* getData(const cJSON* t_server);
void freeUpData(Data *t_data);
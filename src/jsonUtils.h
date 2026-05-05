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

typedef struct {
    char *status;
    char *country;
} Location;

cJSON* readServersJson();
bool validJsonArray(const cJSON *t_json);
void cleanUpJson(cJSON *t_json);

Data* getServerData(const cJSON* t_server);
void freeUpData(Data *t_data);

Location *getLocationData(const char *t_locationStr);
void cleanUpLocationData(Location *t_data);
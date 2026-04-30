#ifndef NETWORK_READ_H
#define NETWORK_READ_H

#include "cjson/cJSON.h"

#define DATA_FILE "speedtest_server_list.json"

typedef struct {
    char* country;
    char* city;
    char* provider;
    char* host;
    int id;
} Data;

typedef int Flags;

Flags readArgs(int argc, char** argv);

cJSON* readJSON(const char* t_path);

Data* getData(const cJSON* t_server);

#endif //NETWORK_READ_H
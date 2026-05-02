#ifndef NETWORK_READ_H
#define NETWORK_READ_H

#include <stdbool.h>

#include "cjson/cJSON.h"

#define DATA_FILE "speedtest_server_list.json"
//#define DATA_FILE "temp.json"

typedef struct {
    char* country;
    char* city;
    char* provider;
    char* host;
    int id;
} Data;

typedef struct {
    int serverId;
    bool downloadData;
} Download;

typedef struct {
    int serverId;
    bool sendData;
} Send;

typedef struct {
    char *seachCountry;
} BestLocation;

typedef struct {
    bool getCurrLocation;
} CurrLocation;

typedef struct {
    Download download;
    Send send;
    BestLocation bestLocation;
    CurrLocation currLocation;
} Config;

Config readArgs(int argc, char** argv);
cJSON* readJSON(const char* t_path);
Data* getData(const cJSON* t_server);

#endif //NETWORK_READ_H
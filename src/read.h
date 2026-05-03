#pragma once

#include <stdbool.h>

#include "cjson/cJSON.h"

//#define DATA_FILE "speedtest_server_list.json"
#define DATA_FILE "temp.json"

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

Config ReadArgs(int argc, char** argv);
cJSON* ReadJSON(const char* t_path);
Data* GetData(const cJSON* t_server);
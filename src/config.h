#pragma once

#include <stdbool.h>

#include "cjson/cJSON.h"

//#define DATA_FILE "data/speedtest_server_list.json"
#define DATA_FILE "data/temp.json"

enum Operation {None, AllServers, SingleServer};
enum LocationType{ NoCountry, CurrentLocation, ProvidedLocation};

typedef struct {
    enum Operation downloadOperation;
    int serverId;

    enum Operation sendOperation;
    int clientId;

    enum LocationType locationType;
    char *searchCountry;

    bool getCurrentLocation;
} Config;

Config readArgs(int argc, char** argv);
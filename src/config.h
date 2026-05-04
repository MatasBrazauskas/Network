#pragma once

#include <stdbool.h>

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
#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    char *data;
    size_t size;
    size_t sent;
} MegaByteOfData;

enum Operation { None, AllServers, SingleServer };
enum LocationType{ NoCountry, ProvidedLocation };

typedef struct {
    enum Operation downloadOperation;
    int serverId;

    enum Operation uploadOperation;
    int clientId;

    enum LocationType locationType;
    char *searchCountry;

    bool getCurrentLocation;
} Config;

Config createConfig(int argc, char** argv);
MegaByteOfData *createMegaByteOfData();
void cleanUpMegaByteOfData(MegaByteOfData *data);
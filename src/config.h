#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "jsonUtils.h"

typedef struct {
    char *data;
    size_t size;
    size_t sent;
} KilobyteOfData;

typedef struct {
    char *serverData;
    float downloadSpeed;
    float uploadSpeed;
} ServerSpeedData;

typedef struct {
    char* downloadServer;
    float downloadSpeed;
    char *uploadServer;
    float uploadSpeed;
} BestServerSpeedData;

enum Operation { None, AllServers, SingleServer };
enum LocationType{ NoCountry, ProvidedLocation };

typedef struct {
    enum Operation downloadOperation;
    int downloadServerId;

    enum Operation uploadOperation;
    int uploadServerId;

    enum LocationType locationType;
    char *searchCountry;

    bool getCurrentLocation;
} Config;

Config createConfig(int argc, char** argv);

KilobyteOfData *createKilobyteOfData();
void cleanUpKilobyte(KilobyteOfData *data);

ServerSpeedData *createServerSpeedData(const Data *t_data, const double t_downloadSpeed, const double t_uploadSpeed);
void cleanUpServerSpeedData(ServerSpeedData *t_data);

BestServerSpeedData createBestServerSpeedData();
void cleanUpBestServerSpeedData(BestServerSpeedData *t_data);

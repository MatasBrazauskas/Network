#include "config.h"

#include "curl/curl.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define COUPLE_KB 64 * 1024;

static Config defaultConfigSetup() {
    const Config config = {
        .downloadOperation = None,
        .downloadServerId = INT_MIN,
        .uploadOperation = None,
        .uploadServerId = INT_MIN,
        .locationType = NoCountry,
        .searchCountry = NULL,
        .getCurrentLocation = false,
    };

    return config;
}

static Config allOperationConfigSetup() {
    const Config config = {
        .downloadOperation = AllServers,
        .downloadServerId = INT_MIN,
        .uploadOperation = AllServers,
        .uploadServerId = INT_MIN,
        .locationType = NoCountry,
        .searchCountry = NULL,
        .getCurrentLocation = true,
    };

    return config;
}

Config createConfig(int argc, char** argv) {
    Config config = defaultConfigSetup();

    bool parsedArgument = false;
    char val;

    while ((val = getopt(argc, argv, "d:u:l:c")) != -1) {
        switch (val) {
            case 'd':
                if (strncmp(optarg, "a", 1) == 0) {
                    config.downloadOperation = AllServers;
                    parsedArgument = true;
                    break;
                }

                const long serverId = strtol(optarg, NULL, 10);

                if (errno == ERANGE || errno == EINVAL || serverId <= 0 || serverId == LONG_MAX) {
                    fprintf(stderr,"Invalid server id.\n");
                    break;
                }

                config.downloadServerId = serverId;
                config.downloadOperation = SingleServer;

                parsedArgument = true;
                break;
            case 'u':
                if (strncmp(optarg, "a", 1) == 0) {
                    config.uploadOperation = AllServers;
                    parsedArgument = true;
                    break;
                }

                const long clientId = strtol(optarg, NULL, 10);

                if (errno == ERANGE || errno == EINVAL || clientId <= 0 || clientId == LONG_MAX) {
                    fprintf(stderr,"Invalid client id.\n");
                    break;
                }

                config.uploadServerId = clientId;
                config.uploadOperation = SingleServer;

                parsedArgument = true;
                break;
            case 'l':
                config.locationType = ProvidedLocation;
                config.searchCountry = optarg;

                parsedArgument = true;
                break;
            case 'c':
                config.getCurrentLocation = true;
                parsedArgument = true;
                break;
            default:
                fprintf(stderr, "Invalid option.\n");
                break;
        }
    }

    if (parsedArgument == false) {
        printf("No arguments provided: running al possible tests.\n");
        config = allOperationConfigSetup();
    }

    return config;
}

KilobytesOfData *createKilobyteOfData() {
    KilobytesOfData *kBData = malloc(sizeof(KilobytesOfData));
    if (kBData == NULL) {
        return NULL;
    }

    const long kB = COUPLE_KB;
    kBData->data = malloc(kB + 1);

    if (kBData->data == NULL) {
        free(kBData);
        return NULL;
    }

    memset(kBData->data, 'A', kB);
    kBData->data[kB] = 0;
    kBData->size = kB;
    kBData->sent = 0;

    return kBData;
}

void cleanUpKilobyte(KilobytesOfData *t_data) {
    if (t_data != NULL) {
        free(t_data->data);
        free(t_data);
    }
}

ServerSpeedData *createServerSpeedData(const Data *t_data, const double t_downloadSpeed, const double t_uploadSpeed) {
    if (t_data == NULL || t_data->host == NULL) {
        return NULL;
    }

    ServerSpeedData *serverSpeedData = malloc(sizeof(ServerSpeedData));
    if (serverSpeedData == NULL) {
        return NULL;
    }

    int length = snprintf(NULL, 0, "%d, %s", t_data->id, t_data->host);

    if (length < 0) {
        cleanUpServerSpeedData(serverSpeedData);
        return NULL;
    }

    serverSpeedData->serverData = malloc((size_t)length);

    if (serverSpeedData->serverData == NULL) {
        cleanUpServerSpeedData(serverSpeedData);
        return NULL;
    }

    snprintf(serverSpeedData->serverData, (size_t)length,"%d, %s", t_data->id, t_data->host);

    serverSpeedData->downloadSpeed = t_downloadSpeed;
    serverSpeedData->uploadSpeed = t_uploadSpeed;

    return serverSpeedData;
}

void cleanUpServerSpeedData(ServerSpeedData *t_data) {
    if (t_data != NULL) {
        free(t_data->serverData);
        free(t_data);

        t_data = NULL;
    }
}

BestServerSpeedData createBestServerSpeedData() {
    const BestServerSpeedData bestServerData = {
        .downloadServer = NULL,
        .downloadSpeed = -1.0F,
        .uploadServer = NULL,
        .uploadSpeed = -1.0F,
    };

    return bestServerData;
}

void cleanUpBestServerSpeedData(BestServerSpeedData *t_data) {
    if (t_data != NULL) {
        free(t_data->downloadServer);
        free(t_data->uploadServer);

        t_data = NULL;
    }
}

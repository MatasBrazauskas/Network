#include "config.h"

#include "curl/curl.h"

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

static Config defaultConfigSetup() {
    const Config config = {
        .downloadOperation = None,
        .serverId = INT_MIN,
        .uploadOperation = None,
        .clientId = INT_MIN,
        .locationType = NoCountry,
        .searchCountry = NULL,
        .getCurrentLocation = false,
    };

    return config;
}

static Config allOperationConfigSetup() {
    const Config config = {
        .downloadOperation = AllServers,
        .serverId = INT_MIN,
        .uploadOperation = AllServers,
        .clientId = INT_MIN,
        .locationType = NoCountry,
        .searchCountry = NULL,
        .getCurrentLocation = true,
    };

    return config;
}

Config createConfig(int argc, char** argv) {
    Config config = defaultConfigSetup();

    int parsedArgs = 0;
    char val;

    while ((val = getopt(argc, argv, "d:u:l:c")) != -1) {
        switch (val) {
            case 'd':
                if (strncmp(optarg, "all", 3) == 0) {
                    config.downloadOperation = AllServers;
                    break;
                }

                const long serverId = strtol(optarg, NULL, 10);

                if (errno == ERANGE || errno == EINVAL || serverId <= 0 || serverId == LONG_MIN || serverId == LONG_MAX) {
                    printf("Invalid server id.\n");
                    break;
                }

                config.serverId = serverId;
                config.downloadOperation = SingleServer;
                break;
            case 'u':
                if (strncmp(optarg, "all", 3) == 0) {
                    config.uploadOperation = AllServers;
                    break;
                }

                const long clientId = strtol(optarg, NULL, 10);

                if (errno == ERANGE || errno == EINVAL || clientId <= 0 || clientId == LONG_MIN || clientId == LONG_MAX) {
                    printf("Invalid client id.\n");
                    break;
                }

                config.uploadOperation = SingleServer;
                config.clientId = clientId;
                break;
            case 'l':
                config.locationType = ProvidedLocation;
                printf("%s\n", optarg);
                config.searchCountry = optarg;
                break;
            case 'c':
                config.getCurrentLocation = true;
                break;
            default:
                fprintf(stderr, "Invalid option.\n");
                parsedArgs--;
                break;
        }
        parsedArgs++;
    }

    if (parsedArgs == 0) {
        config = allOperationConfigSetup();
    }

    return config;
}

MegaByteOfData *createMegaByteOfData() {
    MegaByteOfData *mbData = malloc(sizeof(MegaByteOfData));
    if (mbData == NULL) {
        return NULL;
    }
    const long mb = 1024 * 1024;

    mbData->data = malloc(mb);
    if (mbData->data == NULL) {
        return NULL;
    }

    memset(mbData->data, 'A', mb);

    return mbData;
}

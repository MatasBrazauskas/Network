#include "config.h"

#include "curl/curl.h"

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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

static Config allOperationConfigSetup(const char *t_searchCountry) {
    const char *searchCountry = t_searchCountry;
    enum LocationType locationType = CurrentLocation;

    if (searchCountry == NULL) {
        locationType = NoCountry;
    }

    const Config config = {
        .downloadOperation = AllServers,
        .serverId = INT_MIN,
        .uploadOperation = AllServers,
        .clientId = INT_MIN,
        .locationType = locationType,
        .searchCountry = searchCountry,
        .getCurrentLocation = true,
    };

    return config;
}

Config createConfig(int argc, char** argv, const char *t_currentCountry) {
    Config config = defaultConfigSetup();

    int parsedArgs = 0;
    char val;

    while ((val = getopt(argc, argv, "d:u:l:c")) != -1) {
        switch (val) {
            case 'd':
                if (strcmp(optarg, "all") == 0) {
                    config.downloadOperation = AllServers;
                    break;
                }

                const int serverId = atoi(optarg);

                if (serverId <= 0) {
                    printf("Invalid server id\n");
                    break;
                }

                config.serverId = serverId;
                config.downloadOperation = SingleServer;
                break;
            case 'u':
                if (strcmp(optarg, "all") == 0) {
                    config.uploadOperation = AllServers;
                    break;
                }

                const int clientId = atoi(optarg);

                if (clientId <= 0) {
                    printf("Invalid server id\n");
                    break;
                }

                config.uploadOperation = SingleServer;
                config.clientId = clientId;
                break;
            case 'l':
                config.locationType = ProvidedLocation;
                config.searchCountry = optarg;
                break;
            case 'c':
                config.getCurrentLocation = true;
                break;
            default:
                parsedArgs--;
                break;
        }
        parsedArgs++;
    }

    if (parsedArgs == 0) {
        config = allOperationConfigSetup(t_currentCountry);
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

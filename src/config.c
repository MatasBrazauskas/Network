#include "config.h"

#include "curl/curl.h"

#include <stdlib.h>
#include <unistd.h>


Config readArgs(int argc, char** argv) {
    Config config = {0};

    int parsedArgs = 0;
    char val;

    while ((val = getopt(argc, argv, "s::d::l:c")) != -1) {
        switch (val) {
            case 's':
                if (optarg != NULL) {
                    const int serverId = atoi(optarg);

                    if (serverId <= 0) {
                        printf("Invalid server id\n");
                        break;
                    }

                    config.serverId = serverId;
                    config.downloadOperation = SingleServer;
                } else {
                    config.downloadOperation = AllServers;
                }

                break;
            case 'd':
                if (optarg != NULL) {
                    const int clientId = atoi(optarg);

                    if (clientId <= 0) {
                        printf("Invalid server id\n");
                        break;
                    }

                    config.sendOperation = SingleServer;
                    config.clientId = clientId;
                } else {
                    config.sendOperation = AllServers;
                }

                break;
            case 'l':
                config.locationType = ProvidedLocation;
                config.searchCountry = optarg;
                break;
            case 'c':
                config.getCurrentLocation = true;
                break;
            default:
                printf("Unknown flag: %c\n", val);
                parsedArgs--;
                break;
        }
        parsedArgs++;
    }

    /*if (parsedArgs == 0) {
        config.download.serverId = INT_MIN;
        config.download.downloadData = true;

        config.send.serverId = INT_MIN;
        config.download.serverId = true;

        config.bestLocation.seachCountry = NULL;

        config.currLocation.getCurrLocation = true;
    }*/

    return config;
}
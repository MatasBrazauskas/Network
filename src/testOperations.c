#include "testOperations.h"

#include "curlUtils.h"

#include <stdio.h>
#include <stdlib.h>

bool performDownloadTest(const Config *t_config) {
    return t_config->downloadOperation != None;
}

bool performUploadTest(const Config *t_config) {
    return t_config->uploadOperation != None;
}

bool searchBestServerInCountry(const Config *t_config) {
    return t_config->locationType != NoCountry;
}

bool getCurrentCountry(const Config *t_config) {
    return t_config->getCurrentLocation;
}

bool performDownloadTestOnSingleServer(const Config *t_config) {
    return t_config->downloadOperation == SingleServer;
}

bool performDownloadTestOnAllServers(const Config *t_config) {
    return t_config->downloadOperation == AllServers;
}

bool performUploadTestOnSingleServer(const Config *t_config) {
    return t_config->uploadOperation == SingleServer;
}

bool performUploadTestOnAllServers(const Config *t_config) {
    return t_config->uploadOperation == AllServers;
}

void downloadSingleServer(const Config *t_config) {

}

void downloadAllServers(const Config *t_config) {

}

void uploadSingleServer(const Config *t_config) {

}

void uploadAllServers(const Config *t_config) {

}

void searchForBestServer(const Config *t_config) {

}

void currentCountry() {
    char *currentLocation = NULL;
    CURL *locationCurl = createLocationCurl();

    if (locationCurl != NULL) {
        currentLocation = getCurrLocation(locationCurl);

        if (currentLocation == NULL) {
            printf("Some problem getting the user location.\n");
        } else {
            printf("Current user location: %s.\n", currentLocation);
        }
    } else {
        printf("Can't initialize CURL for connection with location API.\n");
    }

    cleanUpCurl(locationCurl);
    free(currentLocation);
}

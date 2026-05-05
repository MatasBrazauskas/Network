#include "testOperations.h"

#include "curlUtils.h"

#include <stdio.h>
#include <stdlib.h>

#include "jsonUtils.h"

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

void downloadSingleServer(const Config *t_config, const Data *t_data) {
    if (t_config->serverId == t_data->id) {
        downloadAllServers(t_data);
    }
}

void downloadAllServers(const Data *t_data) {
    CURL* downloadCurl = createDownloadCurl(t_data->host);
    if (downloadCurl == NULL) {
        return;
    }

    char *url = setUrl(downloadCurl, t_data->host, DownloadUrl);
    if (url == NULL) {
        cleanUpCurl(downloadCurl);
        return;
    }

    const double download_mbps = downloadSpeed(downloadCurl);
    if (download_mbps == 0.0F) {
        return;
    }
    printf("Download Mbs per second: %f\n", download_mbps);

    free(url);
    cleanUpCurl(downloadCurl);
}

void uploadSingleServer(const Config *t_config, const Data *t_data, MegaByteOfData *t_megaByteOfData) {
    if (t_config->clientId == t_data->id) {
        uploadAllServers(t_data, t_megaByteOfData);
    }
}

void uploadAllServers(const Data *t_data, MegaByteOfData *t_megaByteOfData) {
    CURL *uploadCurl = createUploadCurl(t_data->host);
    if (uploadCurl == NULL) {
        return;
    }

    char *url = setUrl(uploadCurl, t_data->host, UploadUrl);
    if (url == NULL) {
        cleanUpCurl(uploadCurl);
        return;
    }

    const double upload_mbps = uploadSpeed(uploadCurl, t_megaByteOfData);
    if (upload_mbps == 0.0f) {
        return;
    }

    printf("Upload Mbs per second: %f\n", upload_mbps);

    free(url);
    cleanUpCurl(uploadCurl);
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

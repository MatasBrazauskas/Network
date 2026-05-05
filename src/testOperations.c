#include "testOperations.h"

#include "curlUtils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jsonUtils.h"

static bool updateBestServerData(char **t_bestServerName, float *t_bestSpeed, const float t_candidateSpeed, const char *t_serverData) {
    if (t_bestServerName == NULL || t_bestSpeed == NULL || t_serverData == NULL) {
        return false;
    }

    if (t_candidateSpeed <= 0.0F || t_candidateSpeed <= *t_bestSpeed) {
        return true;
    }

    const size_t len = strlen(t_serverData) + 1;
    char *ptr = realloc(*t_bestServerName, len);
    if (ptr == NULL) {
        return false;
    }

    memcpy(ptr, t_serverData, len);
    *t_bestServerName = ptr;
    *t_bestSpeed = t_candidateSpeed;

    return true;
}

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

double downloadSingleServer(const Config *t_config, const Data *t_data) {
    if (t_config == NULL || t_data == NULL) {
        return 0.0F;
    }

    if (t_config->downloadServerId == t_data->id) {
        return downloadAllServers(t_data);
    }
    return 0.0F;
}

double downloadAllServers(const Data *t_data) {
    if (t_data == NULL || t_data->host == NULL) {
        return 0.0F;
    }

    CURL* downloadCurl = createDownloadCurl();
    if (downloadCurl == NULL) {
        return 0.0F;
    }

    char *url = setUrl(downloadCurl, t_data->host, DownloadUrl);
    if (url == NULL) {
        cleanUpCurl(downloadCurl);
        return 0.0F;
    }

    const double downloadMbps = downloadSpeed(downloadCurl);
    if (downloadMbps == 0.0F) {
        free(url);
        cleanUpCurl(downloadCurl);
        return 0.0F;
    }

    free(url);
    cleanUpCurl(downloadCurl);

    return downloadMbps;
}

double uploadSingleServer(const Config *t_config, const Data *t_data, KilobyteOfData *t_megaByteOfData) {
    if (t_config == NULL || t_data == NULL) {
        return 0.0F;
    }

    if (t_config->uploadServerId == t_data->id) {
        return uploadAllServers(t_data, t_megaByteOfData);
    }
    return 0.0F;
}

double uploadAllServers(const Data *t_data, KilobyteOfData *t_megaByteOfData) {
    if (t_data == NULL || t_data->host == NULL || t_megaByteOfData == NULL) {
        return 0.0F;
    }

    CURL *uploadCurl = createUploadCurl();
    if (uploadCurl == NULL) {
        return 0.0F;
    }

    char *url = setUrl(uploadCurl, t_data->host, UploadUrl);
    if (url == NULL) {
        cleanUpCurl(uploadCurl);
        return 0.0F;
    }

    const double uploadMbps = uploadSpeed(uploadCurl, t_megaByteOfData);
    if (uploadMbps == 0.0f) {
        free(url);
        cleanUpCurl(uploadCurl);
        return 0.0f;
    }

    free(url);
    cleanUpCurl(uploadCurl);

    return uploadMbps;
}
void searchForBestServer(const char *t_currCountry, const Data *t_data, BestServerSpeedData *t_bestServer, const double t_downloadSpeed, const double t_uploadSpeed) {
    if (t_currCountry == NULL || t_data == NULL || t_data->country == NULL || t_bestServer == NULL) {
        return;
    }

    if (strcmp(t_currCountry, t_data->country) != 0) {
        return;
    }

    ServerSpeedData *serverData = createServerSpeedData(t_data, t_downloadSpeed, t_uploadSpeed);

    if (serverData == NULL) {
        return;
    }

    const bool updatedDownload = updateBestServerData(
        &t_bestServer->downloadServer,
        &t_bestServer->downloadSpeed,
        serverData->downloadSpeed,
        serverData->serverData
    );
    const bool updatedUpload = updateBestServerData(
        &t_bestServer->uploadServer,
        &t_bestServer->uploadSpeed,
        serverData->uploadSpeed,
        serverData->serverData
    );

    cleanUpServerSpeedData(serverData);

    if (!updatedDownload || !updatedUpload) {
        return;
    }
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

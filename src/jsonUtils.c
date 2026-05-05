#include "jsonUtils.h"

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static const char Path[] = "data/speedtest_server_list.json";

cJSON *readServersJson() {
    const int fd = open(Path, O_RDONLY);

    if(fd == -1) {
        fprintf(stderr, "Can't open file.");
        return NULL;
    }

    struct stat fileStat;
    if(fstat(fd, &fileStat) == -1) {
        fprintf(stderr, "Can't get file information.");
        return NULL;
    }

    const off_t fileSize = fileStat.st_size;

    void* mappedFile = mmap(NULL, fileSize, PROT_READ, MAP_SHARED, fd, 0);

    if(mappedFile == MAP_FAILED) {
        fprintf(stderr, "Can't create a file mapping.");
        return NULL;
    }

    cJSON* json = cJSON_ParseWithLength(mappedFile, fileSize);

    munmap(mappedFile, fileSize);
    close(fd);

    return json;
}

Data *getServerData(const cJSON *t_server) {
    const cJSON *country = cJSON_GetObjectItemCaseSensitive(t_server, "country");
    const cJSON *city = cJSON_GetObjectItemCaseSensitive(t_server, "city");
    const cJSON *provider = cJSON_GetObjectItemCaseSensitive(t_server, "provider");
    const cJSON *host = cJSON_GetObjectItemCaseSensitive(t_server, "host");
    const cJSON *id = cJSON_GetObjectItemCaseSensitive(t_server, "id");

    if(!cJSON_IsString(country)) {
        return NULL;
    }

    if(!cJSON_IsString(city)) {
        return NULL;
    }

    if(!cJSON_IsString(provider)) {
        return NULL;
    }

    if(!cJSON_IsString(host)) {
        return NULL;
    }

    if(!cJSON_IsNumber(id)) {
        return NULL;
    }

    Data* data = malloc(sizeof(Data));

    if (data == NULL) {
        return NULL;
    }

    data->country = country->valuestring;
    data->city = city->valuestring;
    data->provider = provider->valuestring;
    data->host = host->valuestring;
    data->id = id->valueint;

    return data;
}

bool validJsonArray(const cJSON *t_json) {
    if(t_json == NULL) {
        return false;
    }

    return cJSON_IsArray(t_json);
}

void cleanUpJson(cJSON *t_json) {
    if(t_json != NULL) {
        cJSON_Delete(t_json);
        t_json = NULL;
    }
}

void freeUpData(Data *t_data) {
    if (t_data != NULL) {
        free(t_data);
        t_data = NULL;
    }
}

Location *getLocationData(const char *t_locationStr) {
    cJSON* json = cJSON_Parse(t_locationStr);

    const cJSON *status = cJSON_GetObjectItemCaseSensitive(json, "status");
    const cJSON *country = cJSON_GetObjectItemCaseSensitive(json, "country");

    if (!cJSON_IsString(status)) {
        return NULL;
    }

    if (!cJSON_IsString(country)) {
        return NULL;
    }

    Location *location = malloc(sizeof(Location));
    if (location == NULL) {
        return NULL;
    }

    const size_t statusStrLen = strlen(status->valuestring);
    const size_t countryStrLen = strlen(country->valuestring);

    location->status = malloc(statusStrLen + 1);
    if (location->status == NULL) {
        return NULL;
    }

    memcpy(location->status, status->valuestring, statusStrLen);
    location->status[statusStrLen] = 0;

    location->country = malloc(countryStrLen + 1);
    if (location->country == NULL) {
        free(location->status);
        return NULL;
    }

    memcpy(location->country, country->valuestring, countryStrLen);
    location->country[countryStrLen] = 0;

    cleanUpJson(json);

    return location;
}

void cleanUpLocationData(Location *t_data) {
    if (t_data != NULL) {
        free(t_data->status);
        free(t_data->country);
        free(t_data);

        t_data = NULL;
    }
}

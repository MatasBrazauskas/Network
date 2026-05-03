#include "jsonUtils.h"

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>

cJSON* readJSON(const char* t_path) {
    const int fd = open(t_path, O_RDONLY);

    if(fd == -1) {
        perror("Cant open file");
        abort();
    }

    struct stat fileStat;
    if(fstat(fd, &fileStat) == -1) {
        perror("Cant get file info");
        abort();
    }

    const off_t fileSize = fileStat.st_size;

    void* mappedFile = mmap(NULL, fileSize, PROT_READ, MAP_SHARED, fd, 0);

    if(mappedFile == MAP_FAILED) {
        perror("Failed at mmap");
        abort();
    }

    cJSON* json = cJSON_ParseWithLength(mappedFile, fileSize);

    munmap(mappedFile, fileSize);
    close(fd);

    return json;
}

Data *getData(const cJSON *t_server) {
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

bool validJSON(const cJSON* t_json) {
    if(t_json == NULL) {
        return false;
    }

    return cJSON_IsArray(t_json);
}

void freeUpData(Data *t_data) {
    free(t_data->country);
    free(t_data->city);
    free(t_data->provider);
    free(t_data->host);
    free(t_data);
}

void cleanUpJSON(cJSON *t_json) {
    cJSON_Delete(t_json);
}
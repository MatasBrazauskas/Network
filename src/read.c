#include "read.h"

#include "curl/curl.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>

Config readArgs(int argc, char** argv) {
    Config config = {0};

    bool passedArguments = false;
    char val;
    while ((val = getopt(argc, argv, "s::d::l:c")) != -1) {
        switch (val) {
            case 's':
                if (optarg != NULL) {
                    const int serverId = atoi(optarg);
                    if (serverId == 0) {
                        printf("Invalid server id\n");
                        break;
                    }
                    config.send.serverId = serverId;
                }
                passedArguments = true;
                config.send.sendData = true;
                break;
            case 'd':
                if (optarg != NULL) {
                    const int serverId = atoi(optarg);
                    if (serverId == 0) {
                        printf("Invalid server id\n");
                        break;
                    }
                    config.download.serverId = serverId;
                }
                passedArguments = true;
                config.download.downloadData = true;
                break;
            case 'l':
                passedArguments = true;
                config.bestLocation.seachCountry = optarg;
            case 'c':
                passedArguments = true;
                config.currLocation.getCurrLocation = true;
            default:
                printf("Unknown flag: %c\n", val);
                break;
        }
    }

    if (!passedArguments) {
        config.download.serverId = INT_MIN;
        config.download.downloadData = true;

        config.send.serverId = INT_MIN;
        config.download.serverId = true;

        config.bestLocation.seachCountry = NULL;

        config.currLocation.getCurrLocation = true;
    }

    return config;
}

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

bool noArgsFlagValid(char t_letter) {
    return false;
}

bool ArgsFlagValid(char t_letter) {
    return false;
}

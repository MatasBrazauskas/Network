#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <stdbool.h>

#include <curl/curl.h>
#include <cjson/cJSON.h>
#include "read.h"

#define CHKSPEED_VERSION "1.0"

typedef struct {
    char *data;
    size_t size;
    size_t sent;
} UploadData;

size_t writeCallback(char *t_buffer, size_t t_size, size_t t_nmemb, void *t_userData);
size_t readCallback(char *buffer, size_t size, size_t nitems, void *userdata);

bool validJSON(const cJSON* t_json);
void cleanUpJSON(cJSON *t_json);

void freeUpData(Data *t_data);

CURLcode curlGlobalInit(void);
void cleanUpCurl(CURL* t_curl);
CURL *createCurl(const char *t_path);

double downloadSpeed(CURL *t_curl);
double uploadSpeed(CURL *t_curl, UploadData *t_uploadData, const char *t_path);

#endif
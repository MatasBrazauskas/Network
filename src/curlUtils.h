#pragma once

#include <curl/curl.h>

#define CHKSPEED_VERSION "1.0"

typedef struct {
    char *data;
    size_t size;
    size_t sent;
} UploadData;

CURLcode curlGlobalInit(void);
CURL *createCurl(void);
void cleanUpCurl(CURL* t_curl);

double downloadSpeed(CURL *t_curl, const char *t_path);
double uploadSpeed(CURL *t_curl, UploadData *t_uploadData, const char *t_path);
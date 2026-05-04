#pragma once

#include <curl/curl.h>

#define CHKSPEED_VERSION "1.0"

typedef struct {
    char *data;
    size_t size;
    size_t sent;
} UploadData;

char *createDownloadUrl(const char *t_url);
char *createUploadUrl(const char *t_url);

CURLcode curlGlobalInit(void);

CURL *createLocationCurl();
CURL *createDownloadCurl(const char *t_path);
CURL *createUploadCurl(const char *t_path);

void cleanUpCurl(CURL* t_curl);

char *getCurrLocation(CURL *t_curl);
double downloadSpeed(CURL *t_curl);
double uploadSpeed(CURL *t_curl, UploadData *t_uploadData);
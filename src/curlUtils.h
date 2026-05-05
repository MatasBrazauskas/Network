#pragma once

#include "config.h"

#include <curl/curl.h>

#define CHKSPEED_VERSION "1.0"

static const char LocationUrl[] = "http://ip-api.com/json/?fields=status,country";
static const char DownloadUrl[] = "http://%s/speedtest/random1000x1000.jpg";
static const char UploadUrl[] = "http://%s/speedtest/upload.php";

char *setUrl(CURL *curl, const char *t_url, const char *t_path);

CURLcode curlGlobalInit(void);

CURL *createLocationCurl();
CURL *createDownloadCurl();
CURL *createUploadCurl();

void cleanUpCurl(CURL* t_curl);

char *getCurrLocation(CURL *t_curl);
double downloadSpeed(CURL *t_curl);
double uploadSpeed(CURL *t_curl, MegaByteOfData *t_uploadData);
#pragma once

#include "config.h"

static const char locationApiUrl[] = "http://ip-api.com/json/?fields=status,country";
static const char downloadUrl[] = "http://%s/speedtest/random1000x1000.jpg";
static const char uploadUrl[] = "http://%s/speedtest/upload.php";

char *createDownloadUrl(const char *t_url);
char *createUploadUrl(const char *t_url);
void freeUpData(Data *t_data);
void cleanUpJSON(cJSON *t_json);

#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <stdbool.h>

#include <curl/curl.h>
#include <cjson/cJSON.h>

bool validJSON(const cJSON* t_json);
bool validCurl(const CURL* t_curl);

CURL* curlInit(void);
void cleanUp(CURL* t_curl);

#endif //NETWORK_UTILS_H
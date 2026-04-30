#include "utils.h"

#include <curl/curl.h>

bool validJSON(const cJSON* t_json) {
    if(t_json == NULL) {
        return false;
    }

    return cJSON_IsArray(t_json);
}

bool validCurl(const CURL* t_curl) {
    return t_curl != NULL;
}

CURL* curlInit(void) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL* curl = curl_easy_init();
    return curl;
}

void cleanUp(CURL* t_curl) {
    curl_easy_cleanup(t_curl);
    curl_global_cleanup();
}

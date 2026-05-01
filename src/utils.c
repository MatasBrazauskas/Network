#include "utils.h"

#include <curl/curl.h>

#include <stdlib.h>
#include <string.h>

size_t writeCallback(char *t_buffer, size_t t_size, size_t t_nmemb, void *t_userData) {
    const size_t bytes = t_size * t_nmemb;

    long *readBytes = t_userData;
    *readBytes += bytes;

    return bytes;
}

size_t readCallback(char *t_buffer, size_t t_size, size_t t_nitems, void *t_userdata) {
    UploadData *upload = t_userdata;

    const size_t maxBytes = t_size * t_nitems;
    const size_t remaining = upload->size - upload->sent;

    const size_t bytesToSend = remaining < maxBytes ? remaining : maxBytes;

    if (bytesToSend > 0) {
        memcpy(t_buffer, upload->data + upload->sent, bytesToSend);
        upload->sent += bytesToSend;
    }

    return bytesToSend;
}

bool validJSON(const cJSON* t_json) {
    if(t_json == NULL) {
        return false;
    }

    return cJSON_IsArray(t_json);
}

void cleanUpJSON(cJSON *t_json) {
    cJSON_Delete(t_json);
}

void freeUpData(Data *t_data) {
    free(t_data->country);
    free(t_data->city);
    free(t_data->provider);
    free(t_data->host);
    free(t_data);
}

CURLcode curlGlobalInit(void) {
    return curl_global_init(CURL_GLOBAL_DEFAULT);
}

void cleanUpCurl(CURL* t_curl) {
    curl_easy_cleanup(t_curl);
}


CURL *createCurl(const char* t_path) {
    CURL* t_curl = curl_easy_init();

    if (t_curl == NULL) {
        return NULL;
    }

    const char *format = "http://%s/speedtest/random1000x1000.jpg";

    int length = snprintf(NULL, 0, format, t_path);

    if (length < 0) {
        return NULL;
    }

    char *url = malloc((size_t)length + 1);

    if (url == NULL) {
        return NULL;
    }

    snprintf(url, (size_t)length + 1, format, t_path);

    curl_easy_setopt(t_curl, CURLOPT_URL, url);
    curl_easy_setopt(t_curl,CURLOPT_TIMEOUT, 15L);
    curl_easy_setopt(t_curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(t_curl, CURLOPT_CONNECTTIMEOUT, 5L);
    curl_easy_setopt(t_curl, CURLOPT_USERAGENT, CHKSPEED_VERSION);

    return t_curl;
}

double downloadSpeed(CURL *t_curl) {
    long readBytes = 0;

    curl_easy_setopt(t_curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(t_curl, CURLOPT_WRITEDATA, &readBytes);

    if (curl_easy_perform(t_curl) != CURLE_OK) {
        return 0.0;
    }

    double totalTime = 0.0;
    curl_easy_getinfo(t_curl, CURLINFO_TOTAL_TIME, &totalTime);
    const double mbps = ((double)readBytes * 8.0) / totalTime / 1000000.0;

    return mbps;
}

double uploadSpeed(CURL *t_curl, UploadData *t_uploadData, const char *t_path) {
    const char *format = "http://%s/speedtest/upload.php";

    int length = snprintf(NULL, 0, format, t_path);

    if (length < 0) {
        return 0.0f;
    }

    char *url = malloc((size_t)length + 1);

    if (url == NULL) {
        return 0.0f;
    }

    snprintf(url, (size_t)length + 1, format, t_path);

    curl_easy_setopt(t_curl, CURLOPT_URL, url);
    curl_easy_setopt(t_curl, CURLOPT_READFUNCTION, readCallback);
    curl_easy_setopt(t_curl, CURLOPT_READDATA, t_uploadData);
    curl_easy_setopt(t_curl, CURLOPT_POSTFIELDSIZE, (long)t_uploadData->size);

    if (curl_easy_perform(t_curl) != CURLE_OK) {
        return 0.0;
    }

    double totalTime = 0.0;
    curl_easy_getinfo(t_curl, CURLINFO_TOTAL_TIME, &totalTime);
    const double mbps = ((double)t_uploadData->size * 8) / totalTime / 1000000.0;

    return mbps;
}


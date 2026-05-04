#include "curlUtils.h"
#include "jsonUtils.h"

#include <curl/curl.h>

#include <stdlib.h>
#include <string.h>

static const char LocationUrl[] = "http://ip-api.com/json/?fields=status,country";
static const char DownloadUrl[] = "http://%s/speedtest/random1000x1000.jpg";
static const char UploadUrl[] = "http://%s/speedtest/upload.php";

static char *createUrl(const char *t_url, const char *t_path) {
    const int length = snprintf(NULL, 0, t_path, t_url);
    if (length < 0) {
        return NULL;
    }

    char *url = malloc((size_t)length + 1);
    if (url == NULL) {
        return NULL;
    }

    snprintf(url, (size_t)length + 1, t_path, t_url);

    return url;
}

static size_t locationWriteCallback(void *t_buffer, size_t t_size, size_t t_nmemb, void *t_userData) {
    char **locationJson = t_userData;
    const size_t bytes = t_size * t_nmemb;

    char *response = malloc((bytes + 1) * sizeof(char));

    if (response == NULL) {
        return 0;
    }

    memcpy(response, t_buffer, bytes);
    response[bytes] = 0;

    *locationJson = response;

    return bytes;
}

static size_t writeCallback(char *t_buffer, size_t t_size, size_t t_nmemb, void *t_userData) {
    (void)t_buffer;
    (void)t_userData;

    return t_size * t_nmemb;
}

static size_t readCallback(char *t_buffer, size_t t_size, size_t t_nitems, void *t_userdata) {
    MegaByteOfData *upload = t_userdata;

    const size_t maxBytes = t_size * t_nitems;
    const size_t remaining = upload->size - upload->sent;

    const size_t bytesToSend = remaining < maxBytes ? remaining : maxBytes;

    if (bytesToSend > 0) {
        memcpy(t_buffer, upload->data + upload->sent, bytesToSend);
        upload->sent += bytesToSend;
    }

    return bytesToSend;
}


CURL *createLocationCurl() {
    CURL* t_curl = curl_easy_init();

    if (t_curl == NULL) {
        return NULL;
    }

    const char *url =  LocationUrl;

    curl_easy_setopt(t_curl, CURLOPT_URL, url);
    curl_easy_setopt(t_curl, CURLOPT_URL, LocationUrl);
    curl_easy_setopt(t_curl,CURLOPT_TIMEOUT, 10L);
    curl_easy_setopt(t_curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(t_curl, CURLOPT_USERAGENT, CHKSPEED_VERSION);
    curl_easy_setopt(t_curl, CURLOPT_WRITEFUNCTION, locationWriteCallback);

    return t_curl;
}

CURL *createDownloadCurl(const char *t_path) {
    CURL* t_curl = curl_easy_init();

    if (t_curl == NULL) {
        return NULL;
    }

    char *url = createUrl(t_path, DownloadUrl);

    if (url == NULL) {
        return NULL;
    }

    curl_easy_setopt(t_curl, CURLOPT_URL, url);
    curl_easy_setopt(t_curl,CURLOPT_TIMEOUT, 15L);
    curl_easy_setopt(t_curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(t_curl, CURLOPT_CONNECTTIMEOUT, 5L);
    curl_easy_setopt(t_curl, CURLOPT_USERAGENT, CHKSPEED_VERSION);
    curl_easy_setopt(t_curl, CURLOPT_WRITEFUNCTION, writeCallback);

    free(url);

    return t_curl;
}

CURL *createUploadCurl(const char *t_path) {
    CURL* t_curl = curl_easy_init();

    if (t_curl == NULL) {
        return NULL;
    }

    char *url = createUrl(t_path, UploadUrl);

    if (url == NULL) {
        return NULL;
    }

    curl_easy_setopt(t_curl, CURLOPT_URL, url);
    curl_easy_setopt(t_curl,CURLOPT_TIMEOUT, 15L);
    curl_easy_setopt(t_curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(t_curl, CURLOPT_CONNECTTIMEOUT, 5L);
    curl_easy_setopt(t_curl, CURLOPT_USERAGENT, CHKSPEED_VERSION);
    curl_easy_setopt(t_curl, CURLOPT_READFUNCTION, readCallback);
    curl_easy_setopt(t_curl, CURLOPT_POST, 1L);

    free(url);

    return t_curl;
}

CURLcode curlGlobalInit(void) {
    return curl_global_init(CURL_GLOBAL_DEFAULT);
}

void cleanUpCurl(CURL* t_curl) {
    curl_easy_cleanup(t_curl);
}

char *getCurrLocation(CURL *t_curl) {
    if (t_curl == NULL) {
        return NULL;
    }

    char *locationJson = NULL;

    curl_easy_setopt(t_curl, CURLOPT_WRITEDATA, &locationJson);

    const CURLcode code = curl_easy_perform(t_curl);

    if (code != CURLE_OK) {
        return NULL;
    }

    Location *location = getLocationData(locationJson);

    if (location == NULL) {
        return NULL;
    }

    if (strcmp(location->status, "success") != 0) {
        return NULL;
    }

    const int countryNameLen = strlen(location->country);
    char *country = malloc(countryNameLen + 1);

    if (country == NULL) {
        return NULL;
    }

    memcpy(country, location->country, countryNameLen);
    country[countryNameLen] = 0;

    freeUpLocationData(location);

    return country;
}

double downloadSpeed(CURL *t_curl) {
    const CURLcode code = curl_easy_perform(t_curl);

    if (code != CURLE_OK) {
        return 0.0;
    }

    double totalTime = 0.0;
    off_t downloadBytes = 0;

    curl_easy_getinfo(t_curl, CURLINFO_TOTAL_TIME, &totalTime);
    curl_easy_getinfo(t_curl, CURLINFO_SIZE_DOWNLOAD_T, &downloadBytes);

    const double mbps = ((double)downloadBytes * 8.0) / totalTime / 1000000.0;

    return mbps;
}

double uploadSpeed(CURL *t_curl, MegaByteOfData *t_uploadData) {
    curl_easy_setopt(t_curl, CURLOPT_READDATA, &t_uploadData);
    curl_easy_setopt(t_curl, CURLOPT_POSTFIELDSIZE, (long)t_uploadData->size);

    if (curl_easy_perform(t_curl) != CURLE_OK) {
        return 0.0;
    }

    double totalTime = 0.0;
    curl_off_t uploadedBytes = 0;

    curl_easy_getinfo(t_curl, CURLINFO_TOTAL_TIME, &totalTime);
    curl_easy_getinfo(t_curl, CURLINFO_SIZE_UPLOAD_T, &uploadedBytes);

    const double mbps = ((double)uploadedBytes * 8) / totalTime / 1000000.0;

    return mbps;
}


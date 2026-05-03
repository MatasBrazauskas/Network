#include "curlUtils.h"

#include <curl/curl.h>
#include "utils.h"

#include <stdlib.h>
#include <string.h>

static const char locationApiUrl[] = "http://ip-api.com/json/?fields=status,country";
static const char downloadUrl[] = "http://%s/speedtest/random1000x1000.jpg";
static const char uploadUrl[] = "http://%s/speedtest/upload.php";

static char *createUrl(const char *t_url, const char *t_path) {
    int length = snprintf(NULL, 0, t_path, t_url);
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

char *createDownloadUrl(const char *t_url) {
    return createUrl(t_url, downloadUrl);
}

char *createUploadUrl(const char *t_url) {
    return createUrl(t_url, uploadUrl);
}

static size_t writeCallback(char *t_buffer, size_t t_size, size_t t_nmemb, void *t_userData) {
    (void)t_buffer;
    (void)t_userData;

    return t_size * t_nmemb;
}

static size_t readCallback(char *t_buffer, size_t t_size, size_t t_nitems, void *t_userdata) {
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


CURLcode curlGlobalInit(void) {
    return curl_global_init(CURL_GLOBAL_DEFAULT);
}

CURL *createCurl(void) {
    CURL* t_curl = curl_easy_init();

    if (t_curl == NULL) {
        return NULL;
    }

    curl_easy_setopt(t_curl,CURLOPT_TIMEOUT, 15L);
    curl_easy_setopt(t_curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(t_curl, CURLOPT_CONNECTTIMEOUT, 5L);
    curl_easy_setopt(t_curl, CURLOPT_USERAGENT, CHKSPEED_VERSION);

    return t_curl;
}

void cleanUpCurl(CURL* t_curl) {
    curl_easy_cleanup(t_curl);
}

double downloadSpeed(CURL *t_curl, const char *t_path) {
    char *url = createDownloadUrl(t_path);

    if (url == NULL) {
        printf("URL is null\n");
        return 0.0;
    }

    printf("%s\n", url);

    curl_easy_setopt(t_curl, CURLOPT_URL, url);
    curl_easy_setopt(t_curl, CURLOPT_WRITEFUNCTION, writeCallback);

    const CURLcode code = curl_easy_perform(t_curl);
    free(url);

    if (code != CURLE_OK) {
        printf("CURL is not OK\n");
        return 0.0;
    }

    double totalTime = 0.0;
    off_t downloadBytes = 0;

    curl_easy_getinfo(t_curl, CURLINFO_TOTAL_TIME, &totalTime);
    curl_easy_getinfo(t_curl, CURLINFO_SIZE_DOWNLOAD_T, &downloadBytes);

    const double mbps = ((double)downloadBytes * 8.0) / totalTime / 1000000.0;

    return mbps;
}

double uploadSpeed(CURL *t_curl, UploadData *t_uploadData, const char *t_path) {
    const char *url = createUploadUrl(t_path);

    printf("%s\n", url);

    curl_easy_setopt(t_curl, CURLOPT_URL, url);
    curl_easy_setopt(t_curl, CURLOPT_READFUNCTION, readCallback);
    curl_easy_setopt(t_curl, CURLOPT_READDATA, t_uploadData);

    curl_easy_setopt(t_curl, CURLOPT_POST, 1L);
    curl_easy_setopt(t_curl, CURLOPT_POSTFIELDSIZE, (long)t_uploadData->size);

    if (curl_easy_perform(t_curl) != CURLE_OK) {
        printf("CURL is not OK\n");
        return 0.0;
    }

    double totalTime = 0.0;
    curl_off_t uploadedBytes = 0;

    curl_easy_getinfo(t_curl, CURLINFO_TOTAL_TIME, &totalTime);
    curl_easy_getinfo(t_curl, CURLINFO_SIZE_UPLOAD_T, &uploadedBytes);

    const double mbps = ((double)uploadedBytes * 8) / totalTime / 1000000.0;

    return mbps;
}


#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

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

char *createUploadUrl(const char *t_url) {
    return createUrl(t_url, uploadUrl);
}

char *createDownloadUrl(const char *t_url) {
    return createUrl(t_url, downloadUrl);
}

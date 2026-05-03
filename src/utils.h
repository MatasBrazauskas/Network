#pragma once

static const char uploadUrl[] = "http://%s/speedtest/upload.php";
static const char downloadUrl[] = "http://%s/speedtest/upload.php";

char *createUploadUrl(const char *t_url);
char *createDownloadUrl(const char *t_url);
#pragma once

#include "config.h"

#include <stdbool.h>

#include "jsonUtils.h"

bool performDownloadTest(const Config *t_config);
bool performUploadTest(const Config *t_config);
bool searchBestServerInCountry(const Config *t_config);
bool getCurrentCountry(const Config *t_config);

bool performDownloadTestOnSingleServer(const Config *t_config);
bool performDownloadTestOnAllServers(const Config *t_config);
bool performUploadTestOnSingleServer(const Config *t_config);
bool performUploadTestOnAllServers(const Config *t_config);

void downloadSingleServer(const Config *t_config, const Data *t_data);
void downloadAllServers(const Data *t_data);
void uploadSingleServer(const Config *t_config, const Data *t_data, MegaByteOfData *t_megaByteOfData);
void uploadAllServers(const Data *t_data, MegaByteOfData *t_megaByteOfData);

void searchForBestServer(const Config *t_config);
void currentCountry();
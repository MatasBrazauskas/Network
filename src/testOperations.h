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

double downloadSingleServer(const Config *t_config, const Data *t_data);
double downloadAllServers(const Data *t_data);
double uploadSingleServer(const Config *t_config, const Data *t_data, KilobyteOfData *t_megaByteOfData);
double uploadAllServers(const Data *t_data, KilobyteOfData *t_megaByteOfData);

void searchForBestServer(const char *t_currCountry, const Data *t_data, BestServerSpeedData *t_bestServer, const double t_downloadSpeed, const double t_uploadSpeed);
void currentCountry();

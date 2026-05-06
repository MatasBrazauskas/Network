#include "config.h"
#include "curlUtils.h"
#include "jsonUtils.h"
#include "testOperations.h"

#include <signal.h>
#include <stdio.h>

int main(const int argc, char** argv) {
	cJSON *json = readServersJson();
	if (!validJsonArray(json)) {
		fprintf(stderr, "Invalid JSON, can't start programme.\n");
		return 1;
	}

	const CURLcode init_code = curlGlobalInit();
	if (init_code != CURLE_OK) {
		fprintf(stderr, "Can't initialize global CURL.\n");
		return 1;
	}

	Config config = createConfig(argc, argv);

	if (getCurrentCountry(&config)) {
		currentCountry();
	}

	KilobytesOfData *kilobytes = NULL;
	if (performUploadTest(&config) || searchBestServerInCountry(&config)) {
		kilobytes = createKilobyteOfData();

		if (kilobytes == NULL) {
			config.uploadOperation = None;
			fprintf(stderr,"Can't create a heap memory of 1kB. Upload measurements will be skipped.\n");
		}
	}

	BestServerSpeedData bestServer = createBestServerSpeedData();

	{
		cJSON* server = NULL;
		cJSON_ArrayForEach(server, json) {

			Data *data = getServerData(server);
			if (data == NULL) {
				continue;
			}

			const bool serverInSearchCountry = isServerInSearchCountry(&config, data);

			if (shouldPrintServerData(&config, data)) {
				printf("Country: %s, city: %s, provider: %s, host: %s, id: %d.\n", data->country, data->city, data->provider, data->host, data->id);
			}

			double downloadMbps = 0.0;
			double uploadMbps = 0.0;

			if (performDownloadTest(&config)) {
				if (performDownloadTestOnAllServers(&config)) {
					downloadMbps = downloadAllServers(data);
					if (downloadMbps != 0.0) {
						printf("Download Mbs: %f\n", downloadMbps);
					} else {
						fprintf(stderr, "Failed to perform download operation.\n");
					}
				} else if (performDownloadTestOnSingleServer(&config)) {
					downloadMbps = downloadSingleServer(&config, data);
					if (downloadMbps > 0.0) {
						printf("Download Mbs: %f\n", downloadMbps);
					} else if (downloadMbps == 0.0) {
						fprintf(stderr, "Failed to perform download operation.\n");
					}
				}
			} else if (searchBestServerInCountry(&config) && serverInSearchCountry) {
				downloadMbps = downloadAllServers(data);
				if (downloadMbps != 0.0) {
					printf("Download Mbs: %f\n", downloadMbps);
				} else {
					fprintf(stderr, "Failed to perform download operation.\n");
				}
			}

			if (performUploadTest(&config)) {
				if (performUploadTestOnAllServers(&config)) {
					uploadMbps = uploadAllServers(data, kilobytes);
					if (uploadMbps != 0.0F) {
						printf("Upload Mbs: %f\n", uploadMbps);
					} else {
						fprintf(stderr, "Failed to perform upload operation.\n");
					}
				} else if (performUploadTestOnSingleServer(&config)) {
					uploadMbps = uploadSingleServer(&config, data, kilobytes);
					if (uploadMbps > 0.0F) {
						printf("Upload Mbs: %f\n", uploadMbps);
					} else if (uploadMbps == 0.0F) {
						fprintf(stderr, "Failed to perform upload operation.\n");
					}
				}
			} else if (searchBestServerInCountry(&config) && serverInSearchCountry) {
				uploadMbps = uploadAllServers(data, kilobytes);
				if (uploadMbps != 0.0F) {
					printf("Upload Mbs: %f\n", uploadMbps);
				} else {
					fprintf(stderr, "Failed to perform upload operation.\n");
				}
			}

			if (searchBestServerInCountry(&config) && serverInSearchCountry) {
				searchForBestServer(config.searchCountry, data, &bestServer, downloadMbps, uploadMbps);
			}

			freeUpData(data);
		}
	}

	if (searchBestServerInCountry(&config)) {
		if (bestServer.downloadServer == NULL && bestServer.uploadServer == NULL) {
			fprintf(stderr, "Couldn't find any server that meets criteria in country: %s.\n", config.searchCountry);
		} else {
			if (bestServer.downloadServer != NULL) {
				printf("Best download server: %s. With speed: %f\n", bestServer.downloadServer, bestServer.downloadSpeed);
			} else {
				fprintf(stderr, "Couldn't find the best server for download in country: %s.\n", config.searchCountry);
			}

			if (bestServer.uploadServer != NULL) {
				printf("Best upload server: %s. With speed: %f\n", bestServer.uploadServer, bestServer.uploadSpeed);
			} else {
				fprintf(stderr, "Couldn't find the best server for upload in country: %s.\n", config.searchCountry);
			}
		}
	}

	cleanUpBestServerSpeedData(&bestServer);
	cleanUpKilobyte(kilobytes);
	cleanUpJson(json);
	return 0;
}

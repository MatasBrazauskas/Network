#include <signal.h>
#include <stdio.h>

#include "config.h"
#include "curlUtils.h"
#include "jsonUtils.h"
#include "testOperations.h"

int main(const int argc, char** argv) {
	cJSON *json = readServersJson();
	if (!validJsonArray(json)) {
		printf("Invalid JSON, can't start programme.\n");
		return 1;
	}

	const CURLcode init_code = curlGlobalInit();
	if (init_code != CURLE_OK) {
		printf("Can't initialize global CURL.\n");
		return 1;
	}

	Config config = createConfig(argc, argv);

	/*printf("%d\n", config.downloadOperation);
	printf("%d\n", config.downloadServerId);
	printf("%d\n", config.uploadOperation);
	printf("%d\n", config.uploadServerId);
	printf("%d\n", config.locationType);
	printf("%s\n",config.searchCountry != NULL ? config.searchCountry : "(none)");
	printf("%d\n", config.getCurrentLocation);*/

	if (getCurrentCountry(&config)) {
		currentCountry();
	}

	KilobyteOfData *kilobyte = NULL;
	if (performUploadTest(&config) || searchBestServerInCountry(&config)) {
		kilobyte = createKilobyteOfData();

		if (kilobyte == NULL) {
			config.uploadOperation = None;
			printf("Can't create a heap memory of 1kB. Upload measurements will be skipped.\n");
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
			//printf("%s, %s, %s, %s, %d\n", data->country, data->city, data->provider, data->host, data->id);

			double downloadMbps = 0.0;
			double uploadMbps = 0.0;

			if (performDownloadTest(&config)) {
				if (performDownloadTestOnAllServers(&config)) {
					downloadMbps = downloadAllServers(data);
					if (downloadMbps != 0.0) {
						printf("Download Mbs per second: %f\n", downloadMbps);
					}
				} else if (performDownloadTestOnSingleServer(&config)) {
					downloadMbps = downloadSingleServer(&config, data);
					if (downloadMbps != 0.0) {
						printf("Download Mbs per second: %f\n", downloadMbps);
					}
				}
			} else if (searchBestServerInCountry(&config)) {
				downloadMbps = downloadAllServers(data);
			}

			if (performUploadTest(&config)) {
				if (performUploadTestOnAllServers(&config)) {
					uploadMbps = uploadAllServers(data, kilobyte);
					if (uploadMbps != 0.0F) {
						printf("Upload Mbs per second: %f\n", uploadMbps);
					}
				} else if (performUploadTestOnSingleServer(&config)) {
					uploadMbps = uploadSingleServer(&config, data, kilobyte);
					if (uploadMbps != 0.0F) {
						printf("Upload Mbs per second: %f\n", uploadMbps);
					}
				}
			} else if (searchBestServerInCountry(&config)) {
				uploadMbps = uploadAllServers(data, kilobyte);
			}

			if (searchBestServerInCountry(&config)) {
				searchForBestServer(config.searchCountry, data, &bestServer, downloadMbps, uploadMbps);
			}

			freeUpData(data);
		}
	}

	if (searchBestServerInCountry(&config)) {
		if (bestServer.downloadServer != NULL && bestServer.uploadServer != NULL) {
			printf("Best download server %s. With speed: %f\n", bestServer.downloadServer, bestServer.downloadSpeed);
			printf("Best download server %s. With speed: %f\n", bestServer.uploadServer, bestServer.uploadSpeed);
		} else {
			printf("Couldn't find server or/and any server that meats criteria.\n");
		}
	}

	cleanUpBestServerSpeedData(&bestServer);
	cleanUpKilobyte(kilobyte);
	cleanUpJson(json);
	return 0;
}

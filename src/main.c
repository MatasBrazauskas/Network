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

	MegaByteOfData *megaByteOfData = createMegaByteOfData();
	if (megaByteOfData == NULL) {
		printf("Can't create a heap memory of 1MB");
	}

	const Config config = createConfig(argc, argv);

	printf("%d\n", config.downloadOperation);
	printf("%d\n", config.serverId);
	printf("%d\n", config.uploadOperation);
	printf("%d\n", config.clientId);
	printf("%d\n", config.locationType);
	printf("%s\n",config.searchCountry != NULL ? config.searchCountry : "(none)");
	printf("%d\n", config.getCurrentLocation);

	if (getCurrentCountry(&config)) {
		currentCountry();
	}

	{
		cJSON* server = NULL;
		cJSON_ArrayForEach(server, json) {

			Data *data = getServerData(server);
			if (data == NULL) {
				continue;
			}
			//printf("%s, %s, %s, %s, %d\n", data->country, data->city, data->provider, data->host, data->id);

			if (performDownloadTest(&config)) {
				if (performDownloadTestOnAllServers(&config)) {
					downloadAllServers(data);
				} else if (performDownloadTestOnSingleServer(&config)) {
					downloadSingleServer(&config, data);
				}
			}

			if (performUploadTest(&config)) {
				if (performUploadTestOnAllServers(&config)) {
					uploadAllServers(data, megaByteOfData);
				} else if (performUploadTestOnSingleServer(&config)) {
					uploadSingleServer(&config, data, megaByteOfData);
				}
			}

			if (searchBestServerInCountry(&config)) {
				searchForBestServer(&config);
			}

			freeUpData(data);
		}
	}

	cleanUpMegaByteOfData(megaByteOfData);
	cleanUpJson(json);
}
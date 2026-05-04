#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "curlUtils.h"
#include "jsonUtils.h"

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

	const MegaByteOfData *megaByteOfData = createMegaByteOfData();
	if (megaByteOfData == NULL) {
		printf("Can't create a heap memory of 1MB");
	}

	char *currentLocation = NULL;
	CURL *locationCurl = createLocationCurl();

	if (locationCurl != NULL) {
		currentLocation = getCurrLocation(locationCurl);

		if (currentLocation == NULL) {
			printf("Some problem getting the user location.\n");
		} else {
			printf("Current user location: %s.\n", currentLocation);
		}
	} else {
		printf("Can't initialize CURL for connection with location API.\n");
	}

	cleanUpCurl(locationCurl);

	Config config = createConfig(argc, argv, currentLocation);

	printf("%d\n", config.downloadOperation);
	printf("%d\n", config.serverId);
	printf("%d\n", config.uploadOperation);
	printf("%d\n", config.clientId);
	printf("%d\n", config.locationType);
	printf("%s\n", config.searchCountry);
	printf("%d\n", config.getCurrentLocation);

	{
		cJSON* server = NULL;
		cJSON_ArrayForEach(server, json) {

			Data *data = getServerData(server);
			if (data == NULL) {
				continue;
			}

			CURL* downloadCurl = createDownloadCurl(data->host);
			if (downloadCurl == NULL) {
				continue;
			}

			printf("%s, %s, %s, %s, %d\n", data->country, data->city, data->provider, data->host, data->id);

			const double download_mbps = downloadSpeed(downloadCurl);
			if (download_mbps == 0.0F) {
				continue;
			}
			printf("Download Mbs per second: %f\n", download_mbps);


			CURL *uploadCurl = createUploadCurl(data->host);
			if (uploadCurl == NULL) {
				continue;
			}

			const double upload_mbps = uploadSpeed(uploadCurl, megaByteOfData);
			if (upload_mbps == 0.0f) {
				continue;
			}

			printf("Upload Mbs per second: %f\n", upload_mbps);

			freeUpData(data);
			cleanUpCurl(downloadCurl);
			cleanUpCurl(uploadCurl);
		}
	}

	cleanUpJson(json);
}
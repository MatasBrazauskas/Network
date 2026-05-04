#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "curlUtils.h"
#include "jsonUtils.h"

int main(const int argc, char** argv) {
	readArgs(argc, argv);

	cJSON *json = readServersJson();
	if (!validJsonArray(json)) {
		printf("Invalid JSON\n");
		abort();
	}

	const CURLcode init_code = curlGlobalInit();
	if (init_code != CURLE_OK) {
		printf("Something wrong with CURL\n");
		abort();
	}


	const long ten_mb = 1024 * 1024;
	char ten_mb_of_char[ten_mb];
	memset(ten_mb_of_char, 'A', ten_mb);

	UploadData upload = {.data = ten_mb_of_char, .size = ten_mb, .sent = 0,};

	CURL *locationCurl = createLocationCurl();

	if (locationCurl == NULL) {
		exit(67);
	}

	const char *currLocation = getCurrLocation(locationCurl);
	printf("%s\n", currLocation);

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

			const double upload_mbps = uploadSpeed(uploadCurl, &upload);
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
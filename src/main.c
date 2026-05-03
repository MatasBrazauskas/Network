#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "curlUtils.h"
#include "utils.h"
#include "jsonUtils.h"

int main(const int argc, char** argv) {
	readArgs(argc, argv);

	cJSON *json = readJSON(DATA_FILE);
	if (!validJSON(json)) {
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

	{
		cJSON* server = NULL;
		cJSON_ArrayForEach(server, json) {

			Data *data = getData(server);
			if (data == NULL) {
				continue;
			}

			CURL* curl = createCurl();
			if (curl == NULL) {
				freeUpData(data);
				cleanUpCurl(curl);
				continue;
			}

			printf("%s, %s, %s, %s, %d\n", data->country, data->city, data->provider, data->host, data->id);

			const double download_mbps = downloadSpeed(curl, data->host);
			if (download_mbps == 0.0F) {
				freeUpData(data);
				cleanUpCurl(curl);
				continue;
			}
			printf("Download Mbs per second: %f\n", download_mbps);


			curl = createCurl();
			if (curl == NULL) {
				freeUpData(data);
				cleanUpCurl(curl);
				continue;
			}

			const double upload_mbps = uploadSpeed(curl, &upload, data->host);
			if (upload_mbps == 0.0f) {
				freeUpData(data);
				cleanUpCurl(curl);
				continue;
			}

			printf("Upload Mbs per second: %f\n", upload_mbps);

			freeUpData(data);
			cleanUpCurl(curl);
		}
	}

	cleanUpJSON(json);
}
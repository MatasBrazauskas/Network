#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src/read.h"
#include "src/utils.h"

int main(const int argc, char** argv) {
	readArgs(argc, argv);

	cJSON *json = readJSON(DATA_FILE);
	if (!validJSON(json)) {
		printf("Invalid JSON\n");
		abort();
	}

	const CURLcode initCode = curlGlobalInit();
	if (initCode != CURLE_OK) {
		printf("Something wrong with CURL\n");
		abort();
	}


	const long MB = 1024 * 1024;
	char *mbOfChars = malloc(sizeof(char) * MB);
	memset(mbOfChars, 'A', MB);

	{
		cJSON* server;
		cJSON_ArrayForEach(server, json) {

			Data *data = getData(server);
			if (data == NULL) {
				continue;
			}

			CURL* curl = createCurl(data->host);
			if (curl == NULL) {
				continue;
			}

			printf("%s, %s, %s, %s, %d\n", data->country, data->city, data->provider, data->host, data->id);

			const double downloadMbps = downloadSpeed(curl);
			if (downloadMbps == 0.0f) {
				continue;
			}
			printf("Download Mbs per second: %f\n", downloadMbps);

			UploadData upload = {
				.data = mbOfChars,
				.size = MB,
				.sent = 0,
			};

			const double uploadMbps = uploadSpeed(curl, &upload, data->host);
			if (uploadMbps == 0.0f) {
				continue;
			}

			printf("Upload Mbs per second: %f\n", uploadMbps);

			freeUpData(data);
			cleanUpCurl(curl);
		}
	}


	free(mbOfChars);
	cleanUpJSON(json);
}

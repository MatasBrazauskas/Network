#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src/read.h"
#include "src/helper.h"
#include "src/utils.h"

int main(const int argc, char** argv) {
	ReadArgs(argc, argv);

	cJSON *json = ReadJSON(DATA_FILE);
	if (!validJSON(json)) {
		printf("Invalid JSON\n");
		abort();
	}

	const CURLcode init_code = curlGlobalInit();
	if (init_code != CURLE_OK) {
		printf("Something wrong with CURL\n");
		abort();
	}


	const long ten_mb =10 * 1024 * 1024;
	char *mb_of_chars = malloc(sizeof(char) * ten_mb);
	memset(mb_of_chars, 'A', ten_mb);

	{
		cJSON* server = NULL;
		cJSON_ArrayForEach(server, json) {

			Data *data = GetData(server);
			if (data == NULL) {
				continue;
			}

			CURL* curl = createCurl(data->host);
			if (curl == NULL) {
				freeUpData(data);
				cleanUpCurl(curl);
				continue;
			}

			printf("%s, %s, %s, %s, %d\n", data->country, data->city, data->provider, data->host, data->id);

			const double download_mbps = downloadSpeed(curl);
			if (download_mbps == 0.0f) {
				freeUpData(data);
				cleanUpCurl(curl);
				continue;
			}
			printf("Download Mbs per second: %f\n", download_mbps);

			UploadData upload = {
				.data = mb_of_chars,
				.size = ten_mb,
				.sent = 0,
			};

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


	free(mb_of_chars);
	cleanUpJSON(json);
}

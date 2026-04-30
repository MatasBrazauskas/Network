#include <stdio.h>
#include <stdlib.h>

#include "src/read.h"
#include "src/utils.h"

int main(int argc, char** argv) {
	readArgs(argc, argv);

	cJSON* json = readJSON(DATA_FILE);
	if (!validJSON(json)) {
		printf("Invalid JSON\n");
		abort();
	}

	CURL* curl = curlInit();
	if (!validCurl(curl)) {
		printf("curl_easy_init failed");
		abort();
	}

	{
		cJSON* server;

		cJSON_ArrayForEach(server, json) {
			Data* data = getData(server);

			//printf("%s, %s, %s, %s, %d\n", data->country, data->city, data->provider, data->host, data->id);

			free(data);
		}
	}

	cleanUp(json);
}

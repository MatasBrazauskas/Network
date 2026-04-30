#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#include <cjson/cJSON.h>
#include <curl/curl.h>

#define DATA_FILE "speedtest_server_list.json"

typedef struct {
	char* country;
	char* city;
	char* provider;
	char* host;
	int id;
} Data;

cJSON* readFile(const char* filePath) {
	int fd = open(filePath, O_RDONLY);

	if(fd == -1) {
		perror("Cant open file");
		abort();
	}

	struct stat fileStat;
	if(fstat(fd, &fileStat) == -1) {
		perror("Cant get file info");
		abort();
	}
	
	const off_t fileSize = fileStat.st_size;

	void* mappedFile = mmap(NULL, fileSize, PROT_READ, MAP_SHARED, fd, 0);

	if(mappedFile == MAP_FAILED) {
		perror("Failed at mmap");
		abort();
	}

	cJSON* json = cJSON_ParseWithLength(mappedFile, fileSize);

	close(fd);
	munmap(mappedFile, fileSize);

	return json;
}

int main(int argc, char** argv) {
	curl_global_init(CURL_GLOBAL_DEFAULT);

	char opt;
	while((opt = getopt(argc, argv, "a:b:matas:")) != -1) {
		switch(opt) {
			case 'a': printf("A: %s\n", optarg); break;
			case 'b': printf("B: %s\n", optarg); break;
		}
	}

	cJSON* json = readFile(DATA_FILE);

	if(json == NULL) {
		printf("JSON obj is null");
		abort();
	}

	if(!cJSON_IsArray(json)) {
		printf("JSON needs to be array");
		abort();
	}

	CURL* curl = curl_easy_init();

	if (curl == NULL) {
		printf("curl_easy_init failed");
		abort();
	}

	cJSON* server;

	cJSON_ArrayForEach(server, json) {
		const cJSON* country = cJSON_GetObjectItemCaseSensitive(server, "country");
		const cJSON* city = cJSON_GetObjectItemCaseSensitive(server, "city");
		const cJSON* provider = cJSON_GetObjectItemCaseSensitive(server, "provider");
		const cJSON* host = cJSON_GetObjectItemCaseSensitive(server, "host");
		const cJSON* id = cJSON_GetObjectItemCaseSensitive(server, "id");

		Data data;
		memset(&data, 0, sizeof(data));

		if(cJSON_IsString(country)) {
			data.country = country->valuestring;
		}

		if(cJSON_IsString(city)) {
			data.city = city->valuestring;
		}

		if(cJSON_IsString(provider)) {
			data.provider = provider->valuestring;
		}

		if(cJSON_IsString(host)) {
			data.host = host->valuestring;
		}

		if(cJSON_IsNumber(id)) {
			data.id = id->valueint;
		}

		//printf("%s, %s, %s, %s, %d\n", data.country, data.city, data.provider, data.host, data.id);

		const char * jsonServer = cJSON_Print(server);

		struct curl_slist* headers = NULL;
		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(headers, "Accept: application/json");


		curl_easy_setopt(curl, CURLOPT_URL, data.provider);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonServer);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			printf("Cant connet to %s\n", data.provider);
		} else {
			printf("Conneted to %s\n", data.provider);
		}
	}

	cJSON_Delete(json);
}

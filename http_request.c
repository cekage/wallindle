//OriginalcodefromDanielStenberg,<daniel@haxx.se>,etal.
//https://curl.haxx.se/libcurl/c/getinmemory.html

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/param.h>

#include <curl/curl.h>

#include "http_request.h"
#include "shared.h"

size_t  WriteMemoryCallback(void* contents, size_t size, size_t nmemb,
                            void* userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct*)userp;

    mem->memory = realloc(mem->memory, mem->size + realsize + 1);

    if (mem->memory == NULL) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

wd_result GetJSON(const char* url, MemoryStruct* jsonresponse) {
    CURL* curl_handle;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);
    /* init the curl session */
    curl_handle = curl_easy_init();
    /* specify URL to get */
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    /* send all data to this function */

    // TODO(k) is it still usefull ?
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)jsonresponse);
    /* some servers don't like requests that are made without a user-agent field, so we provide one */
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT,
                     "agent-ckg-fait-mumuse-avec-libcurl/1.0");
    /* getit! */
    res = curl_easy_perform(curl_handle);

    /* check for error s*/
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        return WNDL_ERROR;
    }

    /* clean up curl stuff*/
    curl_easy_cleanup(curl_handle);
    /* we're done with libcurl, so clean it up*/
    curl_global_cleanup();

    return WNDL_OK;
}

wd_result GetEbook(const char* url, const char* filename) {

    //    printf("store content of \"%s\"\n--> to %s\n", url, filename);

    CURL* curl;
    FILE* fp;
    CURLcode res;
    curl = curl_easy_init();
    bool isfetched = true;

    if (NULL != curl) {
        fp = fopen(filename, "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLOPT_USERAGENT,
                         "agent-ckg-fait-mumuse-avec-libcurl/1.0");

        res = curl_easy_perform(curl);

        if (CURLE_OK == res) {
            isfetched = true;
        } else {
            fprintf(stderr, "curl_easy_perform() failed : %s\n", curl_easy_strerror(res));
            isfetched = false;
        }

        curl_easy_cleanup(curl);
        fclose(fp);
    }

    return isfetched ? WNDL_OK : WNDL_ERROR;
}

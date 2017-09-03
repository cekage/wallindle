/*
        DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
                    Version 2, December 2004

 Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>

 Everyone is permitted to copy and distribute verbatim or modified
 copies of this license document, and changing it is allowed as long
 as the name is changed.

            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

  0. You just DO WHAT THE FUCK YOU WANT TO.
*/

// With Original code from Daniel Stenberg <daniel@haxx.se> et al.
// https://curl.haxx.se/libcurl/c/getinmemory.html

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/param.h>

#include <curl/curl.h>

#include "perform_entries.h"
#include "http_request.h"

#include "shared.h"

/*
 * Function: WriteMemoryCallback
 * ----------------------------
 *   Append "n" bytes of "content" to "userp". With "n"
 *   computed with the size of the content to write.
 *   *Original code from libcurl !*
 *
 *   contents: elements to be written
 *   size: size of one element to write
 *   nmemb: the count of elements to write
 *   userp: a MemoryStruct to be appended
 *
 *   returns: the amount of element written
 */
size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb,
                           void* userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct*)userp;

    mem->memory = realloc(mem->memory, mem->size + realsize + 1);

    if (mem->memory == NULL) {
        // out of memory!
        fprintf(stderr, "not enough memory (realloc returned NULL)\n");
        return 0;
    }

    // Append "realsize" bytes of "content" to memory
    memcpy(&(mem->memory[mem->size]), contents, realsize);

    mem->size += realsize;      // update MemoryStruct size
    mem->memory[mem->size] = 0; // Add null terminator (String)

    return realsize;
}


/*
 * Function: GetJSON
 * ----------------------------
 *   Download content at "url" and store it on jsonresponse
 *
 *   url: an url (probably http://domain.tld/
 *   jsonresponse: a MemoryStruct to be filled
 *
 *   returns: WNDL_OK if everything goes well, WNDL_ERROR otherwise
 */
wd_result GetJSON(const char* url, const void* jsonresponse) {
    // TODO(k) change proto as follows : char* GetJSON(const char* url)
    //    printf(" url transmise : %s\n", url);

    CURL* curl_handle;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);
    /* init the curl session */
    curl_handle = curl_easy_init();
    /* specify URL to get */
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    /* send all data to this function */

    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, jsonresponse);
    /* some servers don't like requests that are made without a user-agent field, so we provide one */
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, WALLINDLE_USERAGENT);
    /* getit! */
    res = curl_easy_perform(curl_handle);

    /* check for error s*/
    if (CURLE_OK != res) {
        fprintf(stderr, "curl_easy_perform() failed: %d %s\n", res,
                curl_easy_strerror(res));
    }

    /* clean up curl stuff*/
    curl_easy_cleanup(curl_handle);
    /* we're done with libcurl, so clean it up*/
    curl_global_cleanup();

    return (CURLE_OK == res) ? WNDL_OK : WNDL_ERROR;
}

/*
 * Function: GetEbook
 * ----------------------------
 *   Download an url content to a file.
 *
 *   url: an url (probably http://domain.tld/
 *   filename: a file (will be overwritten)
 *
 *   returns: WNDL_OK if everything goes well, WNDL_ERROR otherwise
 */
wd_result GetEbook(const char* url, const char* filename) {

    // Boilerplate for curl stuff
    CURL* curl;
    wd_result result = WNDL_ERROR;

    if (IsEbookAlreadyDownloaded(filename)) {
        return WNDL_OK;
    }

    curl = curl_easy_init();

    if (NULL != curl) {
        // Open the file "filename"
        FILE* file_handle = fopen(filename, "wb");

        // Curl set-up, url, callback & User Agent
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file_handle);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, WALLINDLE_USERAGENT);

        // perform curl
        CURLcode res = curl_easy_perform(curl);

        if (CURLE_OK == res) { // If no error it's ok
            result = WNDL_OK;
        } else {
            // if not, print to stderr
            fprintf(stderr, "curl_easy_perform() failed : %s\n", curl_easy_strerror(res));
        }

        // close opened file
        fclose(file_handle);
    }

    // classic way to clean up a curl
    curl_easy_cleanup(curl);
    return result;
}

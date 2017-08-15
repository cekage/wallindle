//OriginalcodefromDanielStenberg,<daniel@haxx.se>,etal.
//https://curl.haxx.se/libcurl/c/getinmemory.html
#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include "shared.h"

typedef struct MemoryStruct {
    char* memory;
    size_t size;
} MemoryStruct;

size_t  WriteMemoryCallback(void* contents, size_t size, size_t nmemb,
                            void* userp);

wd_result GetJSON(const char* url, MemoryStruct* jsonresponse);

wd_result GetEbook(const char* url, const char* filename);

#endif/*HTTP_REQUEST_H*/

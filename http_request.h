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

#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include "shared.h"

typedef struct MemoryStruct {
    char* memory;
    size_t size;
} MemoryStruct;

size_t  WriteMemoryCallback(void* contents, size_t size, size_t nmemb,
                            void* userp);

wd_result GetJSON(const char* url, const void* jsonresponse);

wd_result GetEbook(const char* url, const char* filename);

#endif/*HTTP_REQUEST_H*/

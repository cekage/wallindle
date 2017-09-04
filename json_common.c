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


//With Original code from Serge Zaitsev http://zserge.com
//https://github.com/zserge/jsmn/blob/master/example/simple.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>

#include <sys/types.h>

#include "shared.h"
#include "configmanager.h"

#include "json_common.h"

static char* _WBReadFile(const char* filename, bool isConfig);

unsigned int _GetTokenCountExt(const char* jsonresponse, jsmntok_t* tokens,
                               unsigned int maxtoken) {

    int token_count;
    jsmn_parser parser;
    jsmn_init(&parser);

    if (NULL == jsonresponse) {
        return 0;
    }

    // jsmn_parse in "count mode"
    token_count = jsmn_parse(&parser, jsonresponse, strlen(jsonresponse),
                                 tokens, maxtoken);

    //    printf(" token_count = %d\n", token_count);

    if (token_count < 0) {
        fprintf(stderr, "Failed to parse JSON : %d\n", token_count);
        token_count = 0;
    }

    return token_count;
}

unsigned int _GetTokenCount(const char* jsonresponse) {
    return _GetTokenCountExt(jsonresponse, NULL, 0);
}

wd_result _JsonEquivTo(const char* json, const jsmntok_t*
                       tok, const char* s) {
    bool result;

    result = (tok->type == JSMN_STRING);
    result &= ((int)strlen(s) == (tok->end - tok->start));
    result &= (0 == strncmp(json + tok->start, s, tok->end - tok->start));

    return (result ? WNDL_OK : WNDL_ERROR);
}

static char* _WBReadFile(const char* filename, bool isConfig) {
    off_t filesize;
    size_t  totalread;

    char* filecontent;
    FILE* file_handle;

    file_handle = fopen(filename, "r");

    if (NULL == file_handle) {
        fprintf(stderr, "Cannot open %s:%s\n", filename, strerror(errno));
        return NULL;
    }

    filesize = GetFileSize(filename, isConfig);
    filecontent = calloc((size_t)filesize + 1UL, sizeof(char));

    if (NULL == filecontent) {
        fprintf(stderr, "Cannot allocate %jd for filecontent\n", (intmax_t) filesize);
        fclose(file_handle);
        return NULL;
    }

    totalread = fread(filecontent, sizeof(char),
                      (size_t)filesize, file_handle);
    fclose(file_handle);

    if ( sizeof(char) > totalread) {
        fprintf(stderr, "Too small content %" PRIuPTR " bytes\n", totalread);
        free(filecontent);
        return NULL;
    }

    return filecontent;
}

// For testing purpose
static char* _TestReadEntriesJsonFile(const char* filename) {
    return _WBReadFile(filename, false);
}

// For testing purpose
static char* _TestReadoAuthJsonFile(const char* filename) {
    return _WBReadFile(filename, true);
}

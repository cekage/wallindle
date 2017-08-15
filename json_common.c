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

wd_result _JsonEquivTo(const char* json, const jsmntok_t*
                       tok, const char* s) {
    bool result;

    result = (tok->type == JSMN_STRING);
    result &= ((int)strlen(s) == (tok->end - tok->start));
    result &= (strncmp(json + tok->start, s, tok->end - tok->start) == 0);

    return (result ? WNDL_OK : WNDL_ERROR);
}

char* WBReadConfigFile(const char*
                       filename) {
    return _WBReadFile(filename, true);
}

char* WBReadEntriesJsonFile(const char*
                            filename) {
    return _WBReadFile(filename, false);
}

char* WBReadoAuthJsonFile(const char* filename) {
    return _WBReadFile(filename, true);
}

static char* _WBReadFile(const char* filename, bool isConfig) {
    FILE* f = fopen(filename, "r");

    if (NULL == f) {
        fprintf(stderr, "Cannot open %s:%s\n", filename, strerror(errno));
        return NULL;
    }

    const off_t filesize = CheckConfSize(filename, isConfig);
    char* filecontent = calloc((unsigned long)filesize + 1UL, sizeof(char));

    if (NULL == filecontent) {
        fprintf(stderr, "Cannot allocate %jd for filecontent\n", (intmax_t) filesize);
        return NULL;
    }

    const  size_t  totalread = fread(filecontent, sizeof(char),
                                    (size_t)filesize, f);
    fclose(f);

    if ( sizeof(char) > totalread) {
        fprintf(stderr, "Too small content %"PRIuPTR" bytes\n", totalread);
        free(filecontent);
        return NULL;
    }

    return filecontent;
}

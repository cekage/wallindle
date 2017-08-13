//OriginalcodefromSergeZaitsevhttp://zserge.com
//https://github.com/zserge/jsmn/blob/master/example/simple.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shared.h"

static int JsonEquivTo(const char* json, const jsmntok_t* tok,  const char* s) {
    int result;

    if (tok->type == JSMN_STRING
            && (int)strlen(s) == (tok->end - tok->start)
            && strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        result = WNDL_OK;
    } else {
        result = WNDL_ERROR;
    }

    return result;
}

static char* WBReadJsonFile(const char*
                            filename) {
    FILE* f = fopen(filename, "r");

    if (NULL == f) {
        fprintf(stderr, "Cannot open %s:%s\n", filename, strerror(errno));
        return NULL;
    }

    const off_t filesize = CheckConfSize(filename);
    char* filecontent = calloc((unsigned long)filesize + 1UL, sizeof(char));

    if (NULL == filecontent) {
        fprintf(stderr, "Cannot allocate %"PRIuPTR" for filecontent\n", filesize);
        return NULL;
    }

    const  size_t  totalread = fread(filecontent, sizeof(char), (size_t)filesize,
                                     f);
    fclose(f);

// TODO(k) Use WB error/ok
    if ( sizeof(char) > totalread) {
        fprintf(stderr, "Too small content %"PRIuPTR" bytes\n", totalread);
        free(filecontent);
        return NULL;
    }

    return filecontent;
}

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

// With original code from SergeZaitsev http://zserge.com
// https://github.com/zserge/jsmn/blob/master/example/simple.c

#include <string.h>
#include <stdio.h>


#include "./lib/jsmn/jsmn.h"


#include "json_common.h"
#include "json_oauth_parse.h"
#include "shared.h"

char* ExtractToken(const char* jsonresponse) {
#define NEXT_ITEM ++index
    int result;

    jsmn_parser parser;
    jsmntok_t tokens[20];/*We expect no more than 20 tokens */

    if (NULL == jsonresponse) {
        return NULL;
    }

    jsmn_init(&parser);
    result = jsmn_parse(&parser, jsonresponse, strlen(jsonresponse), tokens,
                        sizeof(tokens) / sizeof(tokens[0]));

    if (result < 0) {
        // TODO(k) fprintf!
        printf("Failed to parse JSON: %d\n", result);
        return NULL;
    }

    if (result < 1 || tokens[0].type != JSMN_OBJECT) {
        // TODO(k) fprintf!
        printf("Object expected\n");
        return NULL;
    }

    /* Loop over all keys of the root object */
    for (int index = 1; index < result; index++) {
        if (_JsonEquivTo(jsonresponse, &tokens[index], "access_token") == 0) {
            NEXT_ITEM;
            /* We may use strndup() to fetch string value */
            const  size_t  stringlength = (size_t)(tokens[index].end - tokens[index].start);
            char* extracted_token = NULL;
            StoreContent(jsonresponse + tokens[index].start,
                         stringlength, &extracted_token);
            return extracted_token;
        }
    }

    return NULL;
#undef NEXT_ITEM
}

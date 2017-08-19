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

// With original code from Serge Zaitsev http://zserge.com
// https://github.com/zserge/jsmn/blob/master/example/simple.c

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "./lib/jsmn/jsmn.h"


#include "json_common.h"
#include "json_oauth_parse.h"
#include "shared.h"

/*
 * Function: ExtractoAuth2Token
 * ----------------------------
 *   Returns oAuth2 token stored in a json.
 *
 *   jsonobject: a json object as string
 *
 *   returns: a pointer to the token (a string)
 */
char* ExtractoAuth2Token(const char* jsonobject) {
    int result;
    int index;

    /* We expect no more than 20 tokens */
    jsmntok_t tokens[20];

    // Get the count of tokens in jsonobject
    result = _GetTokenCountExt(jsonobject, tokens,
                               sizeof(tokens) / sizeof(tokens[0]));

    // If something went wrong
    if (result < 1) {
        // quit here
        return NULL;
    } else if (tokens[0].type != JSMN_OBJECT) {
        // if it's not an object : error then quit.
        fprintf(stderr, "Object expected\n");
        return NULL;
    }

    /* Loop over all keys of the root object */
    for (index = 1; index < result; ++index) {
        // Check if we encounter a token named "access_token"
        if (_JsonEquivTo(jsonobject, &tokens[index], "access_token") == 0) {
            // So the usefull content is next token
            ++index;
            /* We may use strndup() to fetch string value */
            size_t stringlength = (size_t)(tokens[index].end - tokens[index].start);
            char* extracted_token = NULL;

            // Store content of token in extracted_token and checks result
            if (WNDL_OK == StoreContent(jsonobject + tokens[index].start,
                                        stringlength, &extracted_token)) {
                // if ok return a pointer to the token
                return extracted_token;
            } else {
                // elsewise, free token
                free(extracted_token);
            }
        }
    }
    // Here nothing usefull found -> return an error (NULL)
    return NULL;
}

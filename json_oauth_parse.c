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
    int token_count;
    unsigned int index;
    char* result = NULL;

    /* We expect no more than 20 tokens */
    jsmntok_t tokens[20];

    // Get the count of tokens in jsonobject
    token_count = _GetTokenCountExt(jsonobject, tokens,
                                    sizeof(tokens) / sizeof(tokens[0]));

    // If something went wrong
    if (0 >= token_count) {
        // quit here
        return NULL;
    }

    // We are sure token_count is always positive !

    if (JSMN_OBJECT != tokens[0].type) {
        // if it's not an object : error then quit.
        fprintf(stderr, "Object expected\n");
        return NULL;
    }

    /* Loop over all keys of the root object */
    for (index = 1; index < (unsigned int) token_count; ++index) {
        // Check if we encounter a token named "access_token"
        if (WNDL_OK == _JsonEquivTo(jsonobject, &tokens[index], "access_token")) {
            char* extracted_token = NULL;
            // So the usefull content is next token
            ++index;
            /* We may use strndup() to fetch string value */
            size_t stringlength = (size_t)(tokens[index].end - tokens[index].start);

            // Store content of token in extracted_token and checks result
            if (WNDL_OK == StoreContent(jsonobject + tokens[index].start,
                                        stringlength, &extracted_token)) {
                // if ok return a pointer to the token
                result = extracted_token;
                break;
            } else {
                // otherwise, free token
                free(extracted_token);
            }
        }
    }

    // Here nothing usefull found -> return an error (NULL)
    return result;
}

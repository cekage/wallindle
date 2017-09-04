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

// With Original code from Serge Zaitsev http://zserge.com
// https://github.com/zserge/jsmn/blob/master/example/simple.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>

#include <sys/param.h>

#include "entries_common.h"
#include "json_common.h"
#include "perform_entries.h"

#include "configmanager.h"

#include "json_entries_parse.h"

static jsmntok_t* _AllocateTokens(const char* jsoncontent, size_t count);
static wd_result _BuildTokens(const char* jsonresponse, jsmntok_t* tokens,
                              int token_count);
static WBEntry* _ExtractEntries(long int max_entries, jsmntok_t* tokens,
                                const char* jsonresponse, int token_count, int index);
static void _FindKeysAndStore(jsmntok_t* tokens, const char* jsonresponse,
                              int index, unsigned int entry_index, WBEntry* entries);
static wd_result _GetJsonKeyPosition(const char* jsoncontent,
                                     const jsmntok_t* tokens, unsigned int max_token_count, const char* key,
                                     jsmntype_t tokentype, unsigned int* from);
static int _GetMaxEntries(jsmntok_t* tokens, const char* jsonresponse,
                          int token_count, unsigned int* pindex);
static long int _GetMaxEntriesField(jsmntok_t* tokens, const char* jsonresponse,
                                    unsigned int token_count, unsigned int* pindex, const char* field);

// TODO(k) rething usage of must_continue (previously a return NULL)
WBEntry* JsonGetEntries(const char* jsonresponse) {
    WBEntry* entries = NULL;
    long int max_entries;
    unsigned int index = 1;
    bool must_continue;

    const unsigned int token_count = _GetTokenCount(jsonresponse);

    if (0 == token_count) {
        return entries;
    };

    jsmntok_t* tokens = _AllocateTokens(jsonresponse, (size_t) token_count);
    must_continue = (NULL != tokens);

    if (must_continue
            && WNDL_ERROR == _BuildTokens(jsonresponse, tokens, token_count)) {
        must_continue = false;
    }

    if (must_continue) {
        max_entries = _GetMaxEntries(tokens, jsonresponse, token_count, &index);
        must_continue = max_entries >= 1;

        _GetJsonKeyPosition(jsonresponse, tokens, token_count,
                            "_embedded", JSMN_OBJECT, &index);
        must_continue &= (token_count != index);

        _GetJsonKeyPosition(jsonresponse, tokens, token_count,
                            "items", JSMN_ARRAY, &index);
        must_continue &= (token_count != index);

    }

    if (must_continue) {
        index += 2;
        entries = _ExtractEntries(max_entries, tokens, jsonresponse, token_count,
                                  index);
    }

    free(tokens);
    return entries;
}

static jsmntok_t* _AllocateTokens(const char* jsoncontent, size_t count) {

    jsmntok_t* tokens = calloc(count + 1, sizeof(jsmntok_t));

    if (NULL == tokens) { return NULL; }


    jsmn_parser parser;
    jsmn_init(&parser);
    int token_count = jsmn_parse(&parser, jsoncontent, strlen(jsoncontent),
                                 tokens, count);

    if ((0 > token_count) || ((int) count != token_count)) {
        free(tokens);
        tokens = NULL;
    }

    return tokens;
}

// TODO(k): return WB ERROR / OK
static wd_result _GetJsonKeyPosition(const char* jsoncontent,
                                     const jsmntok_t* tokens, unsigned int max_token_count, const char* key,
                                     jsmntype_t tokentype, unsigned int* from) {
    bool is_key_present;

    do {
        // Check key exists
        is_key_present = (0 == _JsonEquivTo(jsoncontent, &tokens[*from],
                                            key));
        // and if next item
        ++(*from);

        // is a tokentype
        is_key_present &= (tokens[*from].type == tokentype);
    } while (*from < max_token_count && !is_key_present);

    //if (*from == max_token_count) {
    if (!is_key_present) {
        fprintf(stderr, "%s expected but not found\n", key);
    }

    return is_key_present ? WNDL_OK : WNDL_ERROR;
}

//static void _TokenPrint(jsmntok_t t) {
//    printf("Type=%d, start=%d, end=%d, size=%d\n", t.type, t.start, t.end, t.size);
//}


static long int _GetMaxEntriesField(jsmntok_t* tokens, const char* jsonresponse,
                                    unsigned int token_count, unsigned int* pindex, const char* field) {
    unsigned int index = *pindex; // pointer to index
    char* str_value = NULL;
    long int numeric_value;

    // Reaching field
    _GetJsonKeyPosition(jsonresponse, tokens, token_count, field, JSMN_PRIMITIVE,
                        &index);

    // if token_count is reached, then
    if (index == token_count) {
        // field is not found so print error & return zero
        fprintf(stderr, "'%s' field not found\n", field);
        return 0;
    }

    // Copy content in str_value
    StoreContent(jsonresponse + tokens[index].start,
                 MIN(tokens[index].end - tokens[index].start, 4), &str_value);
    // Convert in 10-basis to numeric_value

    // TODO(k):  better check strtol
    numeric_value = strtol(str_value, NULL, 10);
    // Releasing str_value
    free(str_value);
    return numeric_value;
}

static int _GetMaxEntries(jsmntok_t* tokens, const char* jsonresponse,
                          int token_count, unsigned int* pindex) {

    long int total;
    long int pages; // Number of pages

    // Reaching key "pages"
    pages = _GetMaxEntriesField(tokens, jsonresponse, token_count, pindex, "pages");

    // Checking if page number
    if (pages <= 0) {
        fprintf(stderr, "pages primitive >1 expected\n");
        return 0;
    }

    // Reaching key "total"
    total = _GetMaxEntriesField(tokens, jsonresponse, token_count, pindex, "total");

    if (total <= 0) {
        fprintf(stderr, "'total' primitive >1 expected\n");
        return 0;
    }

    return ceil( (double) MIN(total, MAXIMUM_ENTRIES) / pages);
}

static wd_result _BuildTokens(const char* jsonresponse, jsmntok_t* tokens,
                              int token_count) {

    wd_result result = WNDL_OK;
    jsmn_parser parser;
    jsmn_init(&parser);
    int parse_status = jsmn_parse(&parser, jsonresponse, strlen(jsonresponse),
                                  tokens,
                                  token_count);

    if (0 > parse_status ) {
        fprintf(stderr, "bad json structure\n");
        result = WNDL_ERROR;
    } else {
        if (tokens[0].type != JSMN_OBJECT) {
            fprintf(stderr, "Object expected\n");
            result = WNDL_ERROR;
        }
    }

    return result;
}



//static void _printtoken(jsmntok_t token, const char* json) {
//    printf("Token start:%d end:%d size:%d type:%d  --> %.*s\n", token.start,
//           token.end, token.size, token.type, 30, json + token.start);
//}

static WBEntry* _ExtractEntries(long int max_entries, jsmntok_t* tokens,
                                const char* jsonresponse, int token_count, int index) {
#define NEXT_ITEM ++index

    WBEntry* entries;
    unsigned int entry_index;

    entries = calloc(max_entries + 1, sizeof(WBEntry));

    if (NULL == entries)
    { return NULL; }

    entry_index = 0;

    do { // new entry
        entries[entry_index] = (WBEntry) {.id = -1, .created_at = NULL};
        --index;
        int item_end = tokens[index].end;

        //        _printtoken(tokens[index], jsonresponse);
        //        printf("** entry_index= %d : item_end = %d\n",entry_index, tokens[index].end);
        do {
            //            printf("--> index = %d\n",index);
            _FindKeysAndStore(tokens, jsonresponse, index, entry_index, entries);

            NEXT_ITEM;
            //            _printtoken(tokens[index], jsonresponse);
            //            NEXT_ITEM;
            //            _printtoken(tokens[index], jsonresponse);
        } while (index < token_count && tokens[index].start < item_end);

        //        printf("\n** entry_index= %d --> jsonresponse + tokens[%d].start = %.*s\n",entry_index, index, 40,jsonresponse+ tokens[index].start);
        ++entry_index;
        NEXT_ITEM;
        //        _printtoken(tokens[index], jsonresponse);

    } while (index < token_count && entry_index < max_entries);

    entries[entry_index] = (WBEntry) {.id = 0, .created_at = NULL};

    return entries;
#undef NEXT_ITEM
}

static void _FindKeysAndStore(jsmntok_t* tokens, const char* jsonresponse,
                              int index, unsigned int entry_index, WBEntry* entries) {

    // get size of tokens by subtitute start field to end field
    unsigned int token_size = tokens[index].end - tokens[index].start + 1;

    if (_JsonEquivTo(jsonresponse, &tokens[index], "id") == 0) {
        char* value = NULL;
        //        printf("StoreContent : jsonresponse+delta = %.*s  ",20, jsonresponse+tokens[index].start);
        // Point to next token : value of "id"
        ++index;
        // Store token content inside "value"
        StoreContent(jsonresponse + tokens[index].start, token_size, &value);
        //        printf(" &value = %s ", value);
        // Convert string value in entries field "id"
        entries[entry_index].id = strtol(value, NULL, 10);
        //        printf(" entries[%d].id = %lu\n", entry_index, entries[entry_index].id);

        // No more usage of value : releasing
        free(value);
    } else if (_JsonEquivTo(jsonresponse, &tokens[index], "created_at") == 0) {
        // Point to next token : value of "id"
        ++index;
        // Store token content inside "created_at"
        StoreContent(jsonresponse + tokens[index].start, token_size,
                     &entries[entry_index].created_at);
    }
}


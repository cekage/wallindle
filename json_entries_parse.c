// OriginalcodefromSergeZaitsevhttp://zserge.com
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

static int _GetTokenCount(const char* jsonresponse) {
    jsmn_parser parser;
    jsmn_init(&parser);

    // jsmn_parse in "count mode"
    int token_count = jsmn_parse(&parser, jsonresponse, strlen(jsonresponse),
                                 NULL, 0);

    if (token_count < 0) {
        fprintf(stderr, "Failed to parse JSON : %d\n", token_count);
    }

    return token_count;
}

static jsmntok_t* _AllocateTokens(const char* jsoncontent, size_t count) {
//    jsmntok_t* tokens = calloc(count * 10, sizeof(jsmntok_t));

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

static int _GetJsonKeyPosition(const char* jsoncontent, const jsmntok_t* tokens,
                               int max_token_count, const char* key,
                               jsmntype_t tokentype, int from) {
    bool is_key_present;
    int index = from;

    do {
        // Check key exists
        is_key_present = (_JsonEquivTo(jsoncontent, &tokens[index],
                                       key) == 0);
        // and if next item
        ++index;
        // is a tokentype
        is_key_present &= (tokens[index].type == tokentype);
    } while (index < max_token_count && !is_key_present);

    return index;
}

static void _TokenPrint(jsmntok_t t) {
    printf("Type=%d, start=%d, end=%d, size=%d\n", t.type, t.start, t.end, t.size);
}

WBEntry* JsonGetEntries(const char* jsonresponse) {
#define NEXT_ITEM ++index
    int token_count = _GetTokenCount(jsonresponse);

    if (0 >= token_count) { return NULL; }

    jsmntok_t tokens[token_count];
    jsmn_parser parser;
    jsmn_init(&parser);
    jsmn_parse(&parser, jsonresponse, strlen(jsonresponse),
               tokens, token_count);
//    jsmntok_t tokens= _AllocateTokens(jsonresponse,token_count);
//    if (NULL = tokens) { return NULL; }


    if (tokens[0].type != JSMN_OBJECT) {
        printf("Object expected\n");
//        free(tokens);
        return NULL;
    }

    int index = 1;

    /*
     * Searching pages
     */

    index = _GetJsonKeyPosition(jsonresponse, tokens, token_count,
                                "pages", JSMN_PRIMITIVE, index);

    char* str_pages = NULL;
    StoreContent(jsonresponse + tokens[index].start,
                 MIN(tokens[index].end - tokens[index].start, 4),
                 &str_pages);
    long int pages = strtol(str_pages, NULL, 10);
    free(str_pages);

    if (pages < 1) {
        printf("pages primitive >1 expected\n");
//        free(tokens);
        return NULL;
    }

    printf("pages primitive found @ index = %d ---> %.*s\n", index, 20,
           jsonresponse + tokens[index].start);

    /*
     * Searching total
     */
    index = _GetJsonKeyPosition(jsonresponse, tokens, token_count,
                                "total", JSMN_PRIMITIVE, index);

    if (index == token_count) {
        printf("total primitive expected\n");
//        free(tokens);
        return NULL;
    }

    printf("total primitive found @ index = %d ---> %.*s\n", index, 20,
           jsonresponse + tokens[index].start);
    char* str_max_entries = NULL;
    StoreContent(jsonresponse + tokens[index].start,
                 MIN(tokens[index].end - tokens[index].start, 4),
                 &str_max_entries);
    long int max_entries = strtol(str_max_entries, NULL, 10);
    free(str_max_entries);

    if (max_entries < 1) {
        printf("total primitive >1 expected\n");
//        free(tokens);
        return NULL;
    }

    /*
     * Determines max_entries : capped total div by pages
     */
    max_entries = ceil( (double) MIN(max_entries, MAXIMUM_ENTRIES) / pages);

    index = _GetJsonKeyPosition(jsonresponse, tokens, token_count,
                                "_embedded", JSMN_OBJECT, index);

    if (index == token_count) {
        printf("_embedded object expected\n");
//        free(tokens);
        return NULL;
    }

    printf("_embedded object found @ index = %d ---> %.*s\n", index, 20,
           jsonresponse + tokens[index].start);

    index = _GetJsonKeyPosition(jsonresponse, tokens, token_count,
                                "items", JSMN_ARRAY, index);

    if (index == token_count) {
        printf("items array expected\n");
//        free(tokens);
        return NULL;
    }

    printf("items array found @ index = %d ---> %.*s\n", index, 20,
           jsonresponse + tokens[index].start);

//   WBEntry entries[max_entries + 1];
    WBEntry* entries = calloc(max_entries + 1, sizeof(WBEntry));

    printf(" adding true\n");

    printf("@index = %d ---> %.*s\n", index, 20,
           jsonresponse + tokens[index].start);
    NEXT_ITEM;
    NEXT_ITEM;

    printf("DO ! \n");
    unsigned int entry_index = 0;

    do { // new entry
        entries[entry_index] = (WBEntry) {.id = -1, .created_at = NULL};
        --index;
        int item_end = tokens[index].end;
//        printf("\n** FIRST DO ** @%d index = %d ---> %.*s TO %d\n", tokens[index].start,
//               index, 30, //MIN(20,tokens[index].end - tokens[index].start),
//               jsonresponse + tokens[index].start, item_end);

        do {
//            _TokenPrint(tokens[index]);
//            if (++limit<10)
//            printf("@%d index = %d ---> %.*s\n", tokens[index].start, index, MIN(20,
//                    tokens[index].end - tokens[index].start),
//                   jsonresponse + tokens[index].start);

            if (_JsonEquivTo(jsonresponse, &tokens[index], "id") == 0) {
                char* value = NULL;
                NEXT_ITEM;
                StoreContent(jsonresponse + tokens[index].start,
                             tokens[index].end - tokens[index].start, &value);
                entries[entry_index].id = strtol(value, NULL, 10);
                free(value);
                NEXT_ITEM;
                continue;
            } else if (_JsonEquivTo(jsonresponse, &tokens[index], "created_at") == 0) {
                NEXT_ITEM;
                StoreContent(jsonresponse + tokens[index].start,
                             tokens[index].end - tokens[index].start,
                             &entries[entry_index].created_at);
                NEXT_ITEM;
                continue;

            }

//            if (limit<10)
//            printf("end while ? index %d vs token_count %d || item_end %d vs %d\n\n", index,
//                   token_count, tokens[index].start, item_end);
            NEXT_ITEM;

        } while (index < token_count && tokens[index].start < item_end);

//        if (index == token_count) { break; }

//        printf("END WHILE item_end ! \n\n");
//        _PrintEntry(&entries[entry_index]);
//        printf("\n\n");
        ++entry_index;
        NEXT_ITEM;
//        printf("end while ? entry_index %d vs %lu\n", entry_index, max_entries);
    } while (index < token_count && entry_index < max_entries);

//    printf("END WHILE entry_index ! \n\n\n\n");

    entries[entry_index] = (WBEntry) {.id = 0, .created_at = NULL};

    return entries;
#undef NEXT_ITEM
}


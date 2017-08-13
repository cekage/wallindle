// OriginalcodefromSergeZaitsevhttp://zserge.com
// https://github.com/zserge/jsmn/blob/master/example/simple.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "entries_common.h"
#include "json_common.h"
#include "perform_entries.h"


static int JsonEntriesParse(const char* jsonresponse, WBoAuthCred* wbc) {
#define NEXT_ITEM ++index
    int index;
    int token_count;
    jsmn_parser parser;

    // TODO(k) Vérifier le max avec 100 entrées
    jsmntok_t tokens[8192]; /*We expect no more than 8192 tokens */

    jsmn_init(&parser);
    printf("JsonEntriesParse: %.*s\n", (int)MIN(10, strlen(jsonresponse)),
           jsonresponse);
    token_count = jsmn_parse(&parser, jsonresponse, strlen(jsonresponse), tokens,
                             sizeof(tokens) / sizeof(tokens[0]));

    // TODO(k) WNDL_ERROR / WNDL_OK
    if (token_count < 0) {
        fprintf(stderr, "Failed to parse JSON : %d\n", token_count);
        return 1;
    }

    /* Assume the top-level element is an object*/
    if (token_count < 1 || tokens[0].type != JSMN_OBJECT) {
        // TODO(k) printf->fprintf(stderr
        printf("Objectexpected\n");
        return 1;
    }

    // CONSUME UNTIL EMBEDDED!
    index = 1;
    bool is_embedded_present = false;

    do {
        // Checkif_embeddedexists
        is_embedded_present =
            ((JsonEquivTo(jsonresponse, &tokens[index], "_embedded") == 0));
        // andifnextitem
        NEXT_ITEM;
        // isaJSMN_OBJECT
        is_embedded_present &= (tokens[index].type == JSMN_OBJECT);
    } while (index < token_count && !is_embedded_present);

    if (!is_embedded_present) {
        printf("Failed to find correct _embedded\n");
        return 1;
    }

    printf("Fin de while i=%d token_count=%d is_in_embedded=%d\n", index,
           token_count,
           is_embedded_present);
    // CONSUME UNTIL ITEMS!
    bool is_items_present = false;

    do {
        printf("\U0001F511 %d type:%d, size %" PRIuPTR " : %.*s\n", index,
               tokens[index].type,
               (size_t)tokens[index].end - tokens[index].start, MIN(30,
                       tokens[index].end - tokens[index].start),
               jsonresponse + tokens[index].start);
        // Checkifitemsexistsis_items_present=(JsonEquivTo(jsonresponse,&t[index],"items")==0);
        // andifnextitem
        NEXT_ITEM;
        // is a JSMN_OBJECT
        is_items_present &= (tokens[index].type == JSMN_ARRAY);
    } while (index < token_count && !is_items_present);

    if (!is_items_present) {
        // TODO(k)printf->fprintf(stderr
        printf("Failed to find correct items\n");
        return 1;
    }

    const int end_of_items = tokens[index].end;
    printf("Fin de while i=%d token_count=%d is_items_present=%d --> @%d\n", index,
           token_count, is_items_present, end_of_items);
    NEXT_ITEM;

    do {
        printf("\n");
        printf("\U0001F511 %d@%d->@%d type:%d, size %" PRIuPTR " : %.*s", index,
               tokens[index].start, tokens[index].end, tokens[index].type,
               (size_t)tokens[index].end - tokens[index].start,
               MIN(40, tokens[index].end - tokens[index].start),
               jsonresponse + tokens[index].start);
        printf("\n");
        int item_end = tokens[index].end;
        NEXT_ITEM;

        WBEntry an_entry = (WBEntry) {42, 42, 42, calloc(1, 1)};

        do {
            if (JsonEquivTo(jsonresponse, &tokens[index], "created_at") == 0) {
                NEXT_ITEM;
                StoreContent(jsonresponse + tokens[index].start,
                             tokens[index].end - tokens[index].start,
                             &an_entry.created_at);
                NEXT_ITEM;
                continue;
            }

            if (JsonEquivTo(jsonresponse, &tokens[index], "id") == 0) {
                char* value = NULL;
                NEXT_ITEM;
                StoreContent(jsonresponse + tokens[index].start,
                             tokens[index].end - tokens[index].start, &value);
                an_entry.id = atoi(value);
                free(value);
                NEXT_ITEM;
                continue;
            }

            if (JsonEquivTo(jsonresponse, &tokens[index], "is_archived") == 0) {
                NEXT_ITEM;
                an_entry.is_archived = jsonresponse[tokens[index].start] != '0';
                NEXT_ITEM;
                continue;
            }

            if (JsonEquivTo(jsonresponse, &tokens[index], "is_starred") == 0) {
                NEXT_ITEM;
                an_entry.is_starred = (jsonresponse[tokens[index].start] != '0');
                NEXT_ITEM;
                continue;
            }

            NEXT_ITEM;
        } while (index < token_count && tokens[index].start <= item_end);

        printf("[[--> WALLABAG_ENTRY: id=%lu is_archived=%d is_starred=%d "
               "created_at=\"%.*s\"\n",
               an_entry.id, an_entry.is_archived, an_entry.is_starred,
               (int)MIN(30, strlen(an_entry.created_at)), an_entry.created_at);
        PerformEverything(&an_entry, wbc);
        free(an_entry.created_at);
    } while (index < token_count);

    return 0;
#undef NEXT_ITEM
}

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

TEST jsonentries_get_ok() {
    char* jsoncontent;
    jsoncontent = _WBReadEntriesJsonFile("./files/response_ok_total_10.json");
    ASSERT(NULL != jsoncontent);
    free(jsoncontent);
    PASS();
}

TEST jsonentries_get_count() {
    char* jsoncontent;
    jsoncontent = _WBReadEntriesJsonFile("./files/response_ok_total_1.json");
    ASSERT(NULL != jsoncontent);
    int count = _GetTokenCount(jsoncontent);
    ASSERT_EQ(72, count);
    free(jsoncontent);
    PASS();
}

TEST jsonentries_token_allocation() {
    char* jsoncontent;
    jsoncontent = _WBReadEntriesJsonFile("./files/response_ok_total_1.json");
    ASSERT(NULL != jsoncontent);
    int count = _GetTokenCount(jsoncontent);
    jsmntok_t* tokens = _AllocateTokens(jsoncontent, count);
    free(jsoncontent);
    ASSERT(NULL != tokens);
    free(tokens);
    PASS();
}

TEST jsonentries_token_allocation_too_big() {
    char* jsoncontent;
    jsoncontent = _WBReadEntriesJsonFile("./files/response_ok_total_1.json");
    ASSERT(NULL != jsoncontent);
    jsmntok_t* tokens = _AllocateTokens(jsoncontent, 800);
    free(jsoncontent);
    ASSERT_EQ(NULL, tokens);
    free(tokens);
    PASS();
}

TEST jsonentries_token_allocation_too_low() {
    char* jsoncontent;
    jsoncontent = _WBReadEntriesJsonFile("./files/response_ok_total_1.json");
    ASSERT(NULL != jsoncontent);
    jsmntok_t* tokens = _AllocateTokens(jsoncontent, 12);
    free(jsoncontent);
    ASSERT_EQ(NULL, tokens);
    free(tokens);
    PASS();
}

TEST jsonentries_token_search_embedded() {
    char* jsoncontent;
    jsoncontent = _WBReadEntriesJsonFile("./files/response_ok_total_1.json");
    ASSERT(NULL != jsoncontent);
    int count = _GetTokenCount(jsoncontent);
    ASSERT(count > 0);
    jsmntok_t* tokens = _AllocateTokens(jsoncontent, count);
    ASSERT(NULL != tokens);

    int embedded_pos = 1;
    _GetJsonKeyPosition(jsoncontent, tokens, count, "_embedded",
                                           JSMN_OBJECT, &embedded_pos);
    free(jsoncontent);
    ASSERT_EQ(24, embedded_pos);
    free(tokens);
    PASS();
}

TEST jsonentries_token_search_notembedded() {
    char* jsoncontent;
    jsoncontent = _WBReadEntriesJsonFile("./files/response_ok_total_1.json");
    ASSERT(NULL != jsoncontent);
    int count = _GetTokenCount(jsoncontent);
    ASSERT(count > 0);
    jsmntok_t* tokens = _AllocateTokens(jsoncontent, count);
    ASSERT(NULL != tokens);

    int embedded_pos = 1;
    _GetJsonKeyPosition(jsoncontent, tokens, count, "_notembedded",
                                           JSMN_OBJECT, &embedded_pos);
    free(jsoncontent);
    ASSERT_EQ(count, embedded_pos);
    free(tokens);
    PASS();
}

TEST jsonentries_token_search_items() {
    char* jsoncontent;
    jsoncontent = _WBReadEntriesJsonFile("./files/response_ok_total_1.json");
    ASSERT(NULL != jsoncontent);
    int count = _GetTokenCount(jsoncontent);
    ASSERT(count > 0);
    jsmntok_t* tokens = _AllocateTokens(jsoncontent, count);
    ASSERT(NULL != tokens);
    int key_pos = 1;
    _GetJsonKeyPosition(jsoncontent, tokens, count, "_embedded",
                                  JSMN_OBJECT, &key_pos);
    _GetJsonKeyPosition(jsoncontent, tokens, count, "items", JSMN_ARRAY,
                                  &key_pos);
    free(jsoncontent);
    ASSERT_EQ(26, key_pos);
    free(tokens);
    PASS();
}
TEST jsonentries_token_search_notitems() {
    char* jsoncontent;
    jsoncontent = _WBReadEntriesJsonFile("./files/response_ok_total_1.json");
    ASSERT(NULL != jsoncontent);
    int count = _GetTokenCount(jsoncontent);
    ASSERT(count > 0);
    jsmntok_t* tokens = _AllocateTokens(jsoncontent, count);
    ASSERT(NULL != tokens);
    int key_pos = 1;
    _GetJsonKeyPosition(jsoncontent, tokens, count, "_embedded",
                                  JSMN_OBJECT, &key_pos);
    _GetJsonKeyPosition(jsoncontent, tokens, count, "notitems", JSMN_ARRAY,
                                  &key_pos);
    free(jsoncontent);
    ASSERT_EQ(count, key_pos);
    free(tokens);
    PASS();
}

TEST jsonentries_get_entries() {
    char* jsoncontent;
    jsoncontent = _WBReadEntriesJsonFile("./files/response_ok_total_10.json");
    ASSERT(NULL != jsoncontent);
    WBEntry* entries = JsonGetEntries(jsoncontent);
    ASSERT(NULL != entries);
    // Clean all created_at string
    for (int i = 0; i < MAXIMUM_ENTRIES; ++i) {
        if (0 == entries[i].id) { break; }

        free(entries[i].created_at);
    }

    free(entries);
    free(jsoncontent);
    PASS();
}

TEST jsonentries_perform_entries() {
    char* jsoncontent;
    jsoncontent = _WBReadEntriesJsonFile("./files/response_ok_total_10.json");
    ASSERT(NULL != jsoncontent);
    WBEntry* entries = JsonGetEntries(jsoncontent);
    ASSERT(NULL != entries);
    WBoAuthCred known = (WBoAuthCred) {
        "exAmple.org", "10921029",
        "qlmksd,qcpsdqslkdqs,kdqjsmdl,qj",
        "mqimqsdicmqsx<sqidUOIUZKJSHQS",
        "toto123", ""
    };

    // Clean all created_at string
    for (int i = 0; i < MAXIMUM_ENTRIES; ++i) {
        if (0 == entries[i].id) { break; }
        _PerformEverything(&entries[i], &known);
        free(entries[i].created_at);
    }

    free(entries);
    free(jsoncontent);
    PASS();
}

TEST jsonentries_perform_0_entries() {
    char* jsoncontent;
    jsoncontent = _WBReadEntriesJsonFile("./files/2017-08-15-0700.json");
    ASSERT(NULL != jsoncontent);
    WBEntry* entries = JsonGetEntries(jsoncontent);
    ASSERT(NULL == entries);
    free(entries);
    free(jsoncontent);
    PASS();
}


SUITE(_jsonentries) {
    RUN_TEST(jsonentries_get_ok);
    RUN_TEST(jsonentries_get_count);
    RUN_TEST(jsonentries_token_allocation);
    RUN_TEST(jsonentries_token_allocation_too_big);
    RUN_TEST(jsonentries_token_allocation_too_low);
    RUN_TEST(jsonentries_token_search_embedded);
    RUN_TEST(jsonentries_token_search_notembedded);
    RUN_TEST(jsonentries_token_search_items);
    RUN_TEST(jsonentries_token_search_notitems);
    RUN_TEST(jsonentries_get_entries);
    RUN_TEST(jsonentries_perform_entries);
    RUN_TEST(jsonentries_perform_0_entries);
}

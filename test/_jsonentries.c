TEST jsonentries_get_ok() {
    char* jsoncontent;
    jsoncontent = WBReadEntriesJsonFile("./files/response_ok_total_10.json");
    ASSERT(NULL != jsoncontent);
    free(jsoncontent);
    PASS();
}

TEST jsonentries_get_count() {
    char* jsoncontent;
    jsoncontent = WBReadEntriesJsonFile("./files/response_ok_total_1.json");
    ASSERT(NULL != jsoncontent);
    int count = _GetTokenCount(jsoncontent);
    ASSERT_EQ(72, count);
    free(jsoncontent);
    PASS();
}

TEST jsonentries_token_allocation() {
    char* jsoncontent;
    jsoncontent = WBReadEntriesJsonFile("./files/response_ok_total_1.json");
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
    jsoncontent = WBReadEntriesJsonFile("./files/response_ok_total_1.json");
    ASSERT(NULL != jsoncontent);
    jsmntok_t* tokens = _AllocateTokens(jsoncontent, 800);
    free(jsoncontent);
    ASSERT_EQ(NULL, tokens);
    free(tokens);
    PASS();
}

TEST jsonentries_token_allocation_too_low() {
    char* jsoncontent;
    jsoncontent = WBReadEntriesJsonFile("./files/response_ok_total_1.json");
    ASSERT(NULL != jsoncontent);
    jsmntok_t* tokens = _AllocateTokens(jsoncontent, 12);
    free(jsoncontent);
    ASSERT_EQ(NULL, tokens);
    free(tokens);
    PASS();
}

TEST jsonentries_token_search_embedded() {
    char* jsoncontent;
    jsoncontent = WBReadEntriesJsonFile("./files/response_ok_total_1.json");
    ASSERT(NULL != jsoncontent);
    int count = _GetTokenCount(jsoncontent);
    ASSERT(count > 0);
    jsmntok_t* tokens = _AllocateTokens(jsoncontent, count);
    ASSERT(NULL != tokens);
    int embedded_pos = _GetJsonKeyPosition(jsoncontent, tokens, count, "_embedded",
                                           JSMN_OBJECT, 1);
    free(jsoncontent);
    ASSERT_EQ(24, embedded_pos);
    free(tokens);
    PASS();
}

TEST jsonentries_token_search_items() {
    char* jsoncontent;
    jsoncontent = WBReadEntriesJsonFile("./files/response_ok_total_1.json");
    ASSERT(NULL != jsoncontent);
    int count = _GetTokenCount(jsoncontent);
    ASSERT(count > 0);
    jsmntok_t* tokens = _AllocateTokens(jsoncontent, count);
    ASSERT(NULL != tokens);
    int key_pos = 1;
    key_pos = _GetJsonKeyPosition(jsoncontent, tokens, count, "_embedded",
                                  JSMN_OBJECT, key_pos);
    key_pos = _GetJsonKeyPosition(jsoncontent, tokens, count, "items", JSMN_ARRAY,
                                  key_pos);
    free(jsoncontent);
    ASSERT_EQ(26, key_pos);
    free(tokens);
    PASS();
}

TEST jsonentries_get_entries() {
    char* jsoncontent;
    jsoncontent = WBReadEntriesJsonFile("./files/response_ok_total_10.json");
    ASSERT(NULL != jsoncontent);
    WBEntry* entries = JsonGetEntries(jsoncontent);

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
    jsoncontent = WBReadEntriesJsonFile("./files/response_ok_total_10.json");
    ASSERT(NULL != jsoncontent);
    WBEntry* entries = JsonGetEntries(jsoncontent);

    WBoAuthCred known = (WBoAuthCred) {
        "exAmple.org", "10921029",
        "qlmksd,qcpsdqslkdqs,kdqjsmdl,qj",
        "mqimqsdicmqsx<sqidUOIUZKJSHQS",
        "toto123", ""
    };

    // Clean all created_at string
    for (int i = 0; i < MAXIMUM_ENTRIES; ++i) {
        if (0 == entries[i].id) { break; }

        PerformEverything(&entries[i], &known);
        free(entries[i].created_at);
    }

    free(entries);
    free(jsoncontent);
    PASS();
}

TEST jsonentries_perform_0_entries() {
    char* jsoncontent;
    jsoncontent = WBReadEntriesJsonFile("./files/2017-08-15-0700.json");
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
    RUN_TEST(jsonentries_token_search_items);
    RUN_TEST(jsonentries_get_entries);
    RUN_TEST(jsonentries_perform_entries);
    RUN_TEST(jsonentries_perform_0_entries);
}

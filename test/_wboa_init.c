TEST wboa_init_ok() {
    WBoAuth2Struct wbo;
    int result = WBoAuth2Init(&wbo);
    WBoAuth2Cleanup(&wbo);
    ASSERT_EQ(WNDL_OK, result);
    PASS();
}

// Makes valgrind crying :/
TEST wboa_init_double_init() {
    WBoAuth2Struct wbo;
    int result;
    result = WBoAuth2Init(&wbo);
//    result = WBoAuth2Init(&wbo);
    WBoAuth2Cleanup(&wbo);
    ASSERT_EQ(WNDL_OK, result);
    PASS();
}

TEST wboa_init_double_cleanup() {
    WBoAuth2Struct wbo;
    int result = WBoAuth2Init(&wbo);
    WBoAuth2Cleanup(&wbo);
    WBoAuth2Cleanup(&wbo);
    ASSERT_EQ(WNDL_OK, result);
    PASS();
}

TEST wboa_loop_init_cleanup() {
    WBoAuth2Struct wbo;

    for (int i = 0; i < 8192; ++i) {
        int result = WBoAuth2Init(&wbo);
        WBoAuth2Cleanup(&wbo);
        ASSERT_EQ(WNDL_OK, result);
    }

    PASS();
}

TEST wboa_test_StringSet() {
    WBoAuth2Struct wbo;
    int result = WBoAuth2Init(&wbo);
    int resultset1 = WBoAuth2StringSet(AUTH_URL_MASK, sizeof(AUTH_URL_MASK),
                                       &wbo.access_token);
    int resultset2 = WBoAuth2StringSet(AUTH_URL_MASK, sizeof(AUTH_URL_MASK),
                                       &wbo.refresh_token);
    int resultset3 = WBoAuth2StringSet(AUTH_URL_MASK, sizeof(AUTH_URL_MASK),
                                       &wbo.scope);
    int resultset4 = WBoAuth2StringSet(AUTH_URL_MASK, sizeof(AUTH_URL_MASK),
                                       &wbo.token_type);
    wbo.expires_in = LIMIT_DYNAMIC_STRING;
    WBoAuth2Cleanup(&wbo);
    ASSERT_EQ(WNDL_OK, resultset1);
    ASSERT_EQ(WNDL_OK, resultset2);
    ASSERT_EQ(WNDL_OK, resultset3);
    ASSERT_EQ(WNDL_OK, resultset4);
    ASSERT_EQ(WNDL_OK, result);
    PASS();
}


TEST wboa_test_StringSet_compare() {
    WBoAuth2Struct wbo;
    int result = WBoAuth2Init(&wbo);
    int resultset1 = WBoAuth2StringSet(AUTH_URL_MASK, sizeof(AUTH_URL_MASK),
                                       &wbo.access_token);

    ASSERT_EQ(WNDL_OK, resultset1);
    ASSERT_EQ(WNDL_OK, result);
    ASSERT_STR_EQ(AUTH_URL_MASK, wbo.access_token);
    WBoAuth2Cleanup(&wbo);
    PASS();
}


TEST wboa_test_StringSet_truncate_compare() {
    WBoAuth2Struct wbo;
    int result = WBoAuth2Init(&wbo);
    int resultset1 = WBoAuth2StringSet(AUTH_URL_MASK, sizeof(AUTH_URL_MASK) / 2,
                                       &wbo.access_token);
    ASSERT_STRN_EQ(AUTH_URL_MASK, wbo.access_token, sizeof(AUTH_URL_MASK) / 2);

    WBoAuth2Cleanup(&wbo);
    ASSERT_EQ(WNDL_OK, resultset1);
    ASSERT_EQ(WNDL_OK, result);
    PASS();
}

TEST wboa_test_StringSet_overflow_compare() {
    WBoAuth2Struct wbo;
    int result = WBoAuth2Init(&wbo);
    int resultset1 = WBoAuth2StringSet(AUTH_URL_MASK, sizeof(AUTH_URL_MASK) * 2,
                                       &wbo.access_token);
    ASSERT_STRN_EQ(AUTH_URL_MASK, wbo.access_token, sizeof(AUTH_URL_MASK) * 2);

    WBoAuth2Cleanup(&wbo);
    ASSERT_EQ(WNDL_OK, resultset1);
    ASSERT_EQ(WNDL_OK, result);
    PASS();
}

TEST wboa_test_print() {
    WBoAuth2Struct wbo = (WBoAuth2Struct) {"ok1", "ok2", "ok3", "ok4", 1337};
    WBoAuth2Print(&wbo);
    PASS();
}

SUITE(_wboa_init) {
    RUN_TEST(wboa_init_ok);
    RUN_TEST(wboa_init_double_init);
    RUN_TEST(wboa_init_double_cleanup);
    RUN_TEST(wboa_loop_init_cleanup);
    RUN_TEST(wboa_test_StringSet);
    RUN_TEST(wboa_test_StringSet_compare);
    RUN_TEST(wboa_test_StringSet_truncate_compare);
    RUN_TEST(wboa_test_StringSet_overflow_compare);
    RUN_TEST(wboa_test_print);
}
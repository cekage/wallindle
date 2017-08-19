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

TEST jsonauth_get_ok() {
    const char* EXPECTED_TOKEN =
        "MTZhYTFiNjIwMGJhZmEyMmFlMWExZmU5YmEzZjhkMjU3ZTU0NWI5MTQ2Yjg3YmE2NzNkN2UxOWY5YWUzOWYzOQ";

    char* jsoncontent;
    char* token;

    jsoncontent = _WBReadoAuthJsonFile("./files/token_ok.json");
    ASSERT(NULL != jsoncontent);

    token = ExtractoAuth2Token(jsoncontent);
    ASSERT(NULL != token);
    ASSERT(strcmp(EXPECTED_TOKEN, token) == 0);

    free(jsoncontent);
    free(token);
    PASS();
}

TEST jsonauth_get_nok() {
    const char* EXPECTED_TOKEN = "MTZhYTFiNjIw";

    char* jsoncontent;
    char* token;

    jsoncontent = _WBReadoAuthJsonFile("./files/token_ok.json");
    ASSERT(NULL != jsoncontent);

    token = ExtractoAuth2Token(jsoncontent);
    ASSERT(NULL != token);
    ASSERT(strcmp(EXPECTED_TOKEN, token) != 0);

    free(jsoncontent);
    free(token);
    PASS();
}

TEST jsonauth_get_nok_accesstoken() {
    char* jsoncontent;
    char* token;

    jsoncontent = _WBReadoAuthJsonFile("./files/token_error_accesstoken.json");
    ASSERT(NULL != jsoncontent);

    token = ExtractoAuth2Token(jsoncontent);
    ASSERT(NULL == token);

    free(jsoncontent);
    free(token);
    PASS();
}

TEST jsonauth_get_nok_expiresin() {
    char* jsoncontent;
    char* token;

    jsoncontent = _WBReadoAuthJsonFile("./files/token_error_expiresin.json");
    ASSERT(NULL != jsoncontent);

    token = ExtractoAuth2Token(jsoncontent);
    ASSERT(NULL != token);

    free(jsoncontent);
    free(token);
    PASS();
}

TEST jsonauth_get_nok_refreshtoken() {
    char* jsoncontent;
    char* token;

    jsoncontent = _WBReadoAuthJsonFile("./files/token_error_refreshtoken.json");
    ASSERT(NULL != jsoncontent);

    token = ExtractoAuth2Token(jsoncontent);
    ASSERT(NULL != token);

    free(jsoncontent);
    free(token);
    PASS();
}

TEST jsonauth_get_nok_scope() {
    char* jsoncontent;
    char* token;

    jsoncontent = _WBReadoAuthJsonFile("./files/token_error_scope.json");
    ASSERT(NULL != jsoncontent);

    token = ExtractoAuth2Token(jsoncontent);
    ASSERT(NULL != token);

    free(jsoncontent);
    free(token);
    PASS();
}

TEST jsonauth_get_nok_tokentype() {
    char* jsoncontent;
    char* token;

    jsoncontent = _WBReadoAuthJsonFile("./files/token_error_tokentype.json");
    ASSERT(NULL != jsoncontent);

    token = ExtractoAuth2Token(jsoncontent);
    ASSERT(NULL != token);

    free(jsoncontent);
    free(token);
    PASS();
}

TEST jsonauth_get_nok_notjson() {
    char* jsoncontent;
    char* token;

    jsoncontent = _WBReadoAuthJsonFile("./files/token_error_notjson.json");
    ASSERT(NULL != jsoncontent);

    token = ExtractoAuth2Token(jsoncontent);
    ASSERT(NULL == token);

    free(jsoncontent);
    free(token);
    PASS();
}

TEST jsonauth_get_nok_nonexistant() {
    char* jsoncontent;
    char* token;

    jsoncontent = _WBReadoAuthJsonFile("./files/nonexistant.json");
    ASSERT(NULL == jsoncontent);

    token = ExtractoAuth2Token(jsoncontent);
    ASSERT(NULL == token);

    free(jsoncontent);
    free(token);
    PASS();
}

SUITE(_jsonauth) {
    RUN_TEST(jsonauth_get_ok);
    RUN_TEST(jsonauth_get_nok);
    RUN_TEST(jsonauth_get_nok_accesstoken);
    RUN_TEST(jsonauth_get_nok_expiresin);
    RUN_TEST(jsonauth_get_nok_refreshtoken);
    RUN_TEST(jsonauth_get_nok_scope);
    RUN_TEST(jsonauth_get_nok_tokentype);
    RUN_TEST(jsonauth_get_nok_notjson);
    RUN_TEST(jsonauth_get_nok_nonexistant);
}

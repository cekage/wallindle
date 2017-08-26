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

TEST httprequest_ok() {
    const char* expected = "{\"string\": \"Hello wallindle test :-)\"}";
    const char* url = "http://dsl.cekage.net/wallindle.json";

    MemoryStruct curlresponse = (MemoryStruct) {NULL, 0};

    wd_result result = GetJSON(url, &curlresponse);

    ASSERT_EQ(WNDL_OK, result);
    ASSERT_STR_EQ(expected, curlresponse.memory);

    PASS();
}

TEST httpSrequest_ok() {
    const char* expected = "{\"string\": \"Hello wallindle test :-)\"}";
    const char* url = "https://api.ipify.org/?format=json";

    MemoryStruct curlresponse = (MemoryStruct) {NULL, 0};

    wd_result result = GetJSON(url, &curlresponse);

    ASSERT_EQ(WNDL_OK, result);
    ASSERT_STR_EQ(expected, curlresponse.memory);

    PASS();
}

SUITE(_httprequest) {
    RUN_TEST(httprequest_ok);
    RUN_TEST(httpSrequest_ok);
}

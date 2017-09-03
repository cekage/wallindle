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

TEST entriescommon_geturl() {
    WBoAuthCred wba = (WBoAuthCred) {
        "https://exAmple.org", "400",
        "qlmksd,qcpsdqslkdqs,kdqjsmdl,qj",
        "mqimqsdicmqsx<sqidUOIUZKJSHQS",
        "toto123", "sUpErToKeN"
    };
    #define EXPECTED ("https://exAmple.org/api/entries.json?access_token=sUpErToKeN&perPage=500&page=1&since=")
    char* url = WBEntryFetchingURL(&wba);
    printf("url = %s\n", url);
    ASSERT_STRN_EQ(EXPECTED, url, sizeof(EXPECTED)-1);
    free(url);
    PASS();
}

SUITE(_entriescommon) {
    RUN_TEST(entriescommon_geturl);
}

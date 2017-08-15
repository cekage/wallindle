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



TEST performentries_getfilename() {
    WBEntry wbe = (WBEntry) {false, false, 0x123, "DaTe"};

    char* filename = GetEntryFileName(&wbe);
    printf("filename = %s\n", filename);
    ASSERT_STR_EQ("./entries/00000123.mobi", filename);
    free(filename);
    PASS();
}

TEST performentries_geturl() {
    WBEntry wbe = (WBEntry) {false, false, 0x123, "DaTe"};

    WBoAuthCred wba = (WBoAuthCred) {
        "exAmple.org", "10921029",
        "qlmksd,qcpsdqslkdqs,kdqjsmdl,qj",
        "mqimqsdicmqsx<sqidUOIUZKJSHQS",
        "toto123", "sUpErToKeN"
    };

    char* url = WBConfigForgeDownloadURL(&wbe, &wba);
    printf("url = %s\n", url);
    ASSERT_STR_EQ("http://exAmple.org/api/entries/291/export.mobi?access_token=sUpErToKeN",
                  url);
    free(url);
    PASS();
}

TEST performentries_complete() {
    char* jsoncontent;
    jsoncontent = WBReadEntriesJsonFile("./files/response_ok_total_10.json");
    ASSERT(NULL != jsoncontent);
    WBEntry* entries = JsonGetEntries(jsoncontent);

    WBoAuthCred known = (WBoAuthCred) {
        "exAmple.org", "10921029",
        "qlmksd,qcpsdqslkdqs,kdqjsmdl,qj",
        "mqimqsdicmqsx<sqidUOIUZKJSHQS",
        "toto123", "sUpErToKeN"
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


SUITE(_performentries) {
    RUN_TEST(performentries_getfilename);
    RUN_TEST(performentries_geturl);
    RUN_TEST(performentries_complete);
}

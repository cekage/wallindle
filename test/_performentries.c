//#include "../json_common.h"
//#include "../entries_common.h"
//#include "../json_entries_parse.h"
//#include "../perform_entries.h"

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

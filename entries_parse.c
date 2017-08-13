#include "json_entries_parse.h"
#include "http_request.h"




static void  WBReadJsonEntries(WBoAuthCred* wbc, const char* filename) {
    char* jsoncontent = WBReadJsonFile(filename);
    WBReadDownloadedJsonEntries(jsoncontent, wbc);
    free(jsoncontent);
}

static void  WBReadDownloadedJsonEntries(const char* jsoncontent,
        WBoAuthCred* wbc) {
    printf("--8<--snip--8<--\n%.*s\n--8<--snip--8<--\n", (int)MIN(50,
            strlen(jsoncontent)), jsoncontent);
    JsonEntriesParse(jsoncontent, wbc);
}

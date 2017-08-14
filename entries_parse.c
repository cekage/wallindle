#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/param.h>

#include "json_entries_parse.h"
#include "http_request.h"
#include "configmanager.h"

#include "entries_parse.h"
#include "json_common.h"


void  WBReadJsonEntries(WBoAuthCred* wbc, const char* filename) {
    char* jsoncontent = WBReadConfigFile(filename);
    WBReadDownloadedJsonEntries(jsoncontent, wbc);
    free(jsoncontent);
}

void  WBReadDownloadedJsonEntries(const char* jsoncontent,
                                  WBoAuthCred* wbc) {
    printf("--8<--snip--8<--\n%.*s\n--8<--snip--8<--\n", (int)MIN(50,
            strlen(jsoncontent)), jsoncontent);
    JsonEntriesParse(jsoncontent, wbc);
}

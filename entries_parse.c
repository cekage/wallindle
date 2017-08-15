#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/param.h>

#include "json_entries_parse.h"
#include "http_request.h"
#include "configmanager.h"

#include "entries_parse.h"
#include "json_common.h"

static void _WBReadJsonEntries(const char* filename) {
    char* jsoncontent = WBReadConfigFile(filename);
    WBReadDownloadedJsonEntries(jsoncontent);
    free(jsoncontent);
}

void WBReadDownloadedJsonEntries(const char* jsoncontent) {
    //    printf("--8<--snip--8<--\n%.*s\n--8<--snip--8<--\n", (int)MIN(50,
    //            strlen(jsoncontent)), jsoncontent);
    WBEntry* entries = JsonGetEntries(jsoncontent);

    // Clean all created_at string
    for (int i = 0; i < MAXIMUM_ENTRIES; ++i) {
        if (0 == entries[i].id) { break; }

        free(entries[i].created_at);
    }

    free(entries);
}

#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "shared.h"

#include "configmanager.h"
#include "entries_common.h"
#include "http_request.h"

#include "perform_entries.h"

char* WBConfigForgeDownloadURL(const WBEntry* wbe,
                               const WBoAuthCred* wbcred) {
    const  size_t  url_size = ( sizeof(DOWNLOAD_URL_MASK)
                                - (2 * (sizeof("%s") - 1) + (sizeof("%lu") - 1)) //minusformats
                                + MAX_INT_STRLEN//plusmaxintsize
                                + strlen(wbcred->wallabag_host)
                                + strlen(wbcred->token)
                              ) * sizeof(char);
    char* url = calloc(url_size + 1, sizeof(char));
    snprintf(url, url_size, DOWNLOAD_URL_MASK, wbcred->wallabag_host, wbe->id,
             wbcred->token);
    return url;
}


char* GetEntryFileName(const WBEntry* wbe) {
    const  size_t  realsize = ( sizeof(ENTRY_MASK)
                                - (sizeof("%08lx") - 1)
                                + 8
                              ) * sizeof(char);
    char* result = calloc(realsize + 1, sizeof(char));
    snprintf(result, realsize, ENTRY_MASK, wbe->id);
    return result;
}

bool  IsEntriesAlreadyDownloaded(const WBEntry* wbe) {
    struct stat st;
    bool  result;//=wbe->id>0;
    char* filename = GetEntryFileName(wbe);

    if (stat(filename, &st) == 0) {
        result = (st.st_size > 0);
    } else {
        result = false;
    }

    free(filename);
    return result;
}

bool IsEbookAlreadyDownloaded(const char* ebookfile) {
    struct stat st;
    bool  result = false;

    if (stat(ebookfile, &st) == 0) {
        result = (st.st_size > 0);
    }

    return result;
}

void  PerformEverything(const WBEntry* wbe, const WBoAuthCred* wbcred) {
    EnsureEbookDirExists();

    char* filename = GetEntryFileName(wbe);
    printf("Filename for wbe (entry%lu) = %s\n", wbe->id, filename);
    const bool  is_already_downloaded = IsEntriesAlreadyDownloaded(wbe);
    printf("is_already_downloaded=%s\n", is_already_downloaded ? "true" : "false");

    if (!is_already_downloaded) {
        char* url = WBConfigForgeDownloadURL(wbe, wbcred);
        printf("url=%s\n", url);
        GetEbook(url, filename);
        free(url);
    }

    free(filename);
}

void _PrintEntry(const WBEntry* wbe) {
    printf("_PrintEntry\n");
    printf("is_archived = %d\n", wbe->is_archived);
    printf("is_starred = %d\n", wbe->is_starred);
    printf("id = %lu\n", wbe->id);
    printf("created_at= %s\n", wbe->created_at);

//    typedef struct WBEntry {
//    bool  is_archived;
//    bool  is_starred;
//    unsigned long int id;
//    char* created_at;
//} WBEntry;
}


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
                                + 1 //themasksize1
                                - (2 * sizeof("%s") + sizeof("%lu")) //minusformats
                                + MAX_INT_STRLEN//plusmaxintsize
                                + strlen(wbcred->wallabag_host) + 1
                                + strlen(wbcred->token) + 1
                              ) * sizeof(char);
    printf("\n*url_size=%" PRIuPTR "and sizeof(\"%%s\")%" PRIuPTR, url_size,
           sizeof("%s"));
    char* url = calloc(url_size + 1, sizeof(char));
    snprintf(url, url_size, DOWNLOAD_URL_MASK, wbcred->wallabag_host, wbe->id,
             wbcred->token);
    return url;
}


 char* GetEntryFileName(const WBEntry* wbe) {
    const  size_t  realsize = ( sizeof(ENTRY_MASK) - 1
                                - sizeof("%08lx")
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

 void  PerformEverything(const WBEntry* wbe, const WBoAuthCred* wbcred) {
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

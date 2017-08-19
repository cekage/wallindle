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

static bool IsEntriesAlreadyDownloaded(const WBEntry* wbe);

char* WBConfigForgeDownloadURL(const WBEntry* wbe,
                               const WBoAuthCred* wbcred) {
    char* url;
    int sprinted;

    // Compute size needed to store downloading url from the MASK
    // substracting % format and adding strings size
    const  size_t  url_size = ( sizeof(DOWNLOAD_URL_MASK)
                                - (2 * (sizeof("%s") - 1) + (sizeof("%lu") - 1)) //minusformats
                                + MAX_INT_STRLEN//plusmaxintsize
                                + strlen(wbcred->wallabag_host)
                                + strlen(wbcred->token)
                              ) * sizeof(char);

    // Usage of calloc maybe malloc is sufficient
    url = calloc(url_size + 1, sizeof(char));

    // Check if calloc fails
    if (NULL == url) {
        // In this case, error then quit function
        fprintf(stderr, "Cannot allocate download url");
        return NULL;
    }

    // build url with mask and fields but limit the size
    sprinted = snprintf(url, url_size, DOWNLOAD_URL_MASK, wbcred->wallabag_host,
                        wbe->id, wbcred->token);

    // Check if a minimum size is really snprinted
    if (1 > sprinted) {
        // In this case, displays error, free url and quit
        fprintf(stderr, "Cannot sprintf download url");
        free(url);
        return NULL;
    }

    // Return a pointer to URL, must be freed somewhere later !
    return url;
}


char* GetEntryFileName(const WBEntry* wbe) {
    char* result;
    size_t realsize;

    // We need the length of ENTRY_MASK minus size of parameters
    // and add 8 characters
    realsize = (sizeof(ENTRY_MASK)
                - (sizeof("%08lx") - 1)
                + 8) * sizeof(char);

    // Allocate result with "realsize+1" zeros
    result = calloc(realsize + 1, sizeof(char));

    if (NULL == result) {
        fprintf(stderr, "Cannot calloc entry file name");
    } else {
        int sprinted = snprintf(result, realsize, ENTRY_MASK, wbe->id);

        if (1 > sprinted) {
            fprintf(stderr, "Cannot snprintf entry file name");
            free(result);
            result = NULL;
        }
    }

    return result;
}
// TODO(k) 1/ Rename IsEbookAlreadyDownloaded 2/ use it on IsEntriesAlreadyDownloaded
static bool IsEntriesAlreadyDownloaded(const WBEntry* wbe) {
    struct stat fs_stat = {0};
    bool  result;
    char* filename = GetEntryFileName(wbe);

    if (NULL == filename) {
        result = false;
    } else if (stat(filename, &fs_stat) == 0) {
        result = (fs_stat.st_size > 0);
    } else {
        result = false;
    }

    free(filename);
    return result;
}

bool IsEbookAlreadyDownloaded(const char* ebookfile) {
    struct stat fs_stat = {0};
    bool  result = false;

    if (stat(ebookfile, &fs_stat) == 0) {
        result = (fs_stat.st_size > 0);
    }

    return result;
}

static void _PerformEverything(const WBEntry* wbe, const WBoAuthCred* wbcred) {

    char* filename;
    bool  is_already_downloaded;
    EnsureEbookDirExists();

    filename = GetEntryFileName(wbe);

    if (NULL == filename) {
        return;
    }

    is_already_downloaded = IsEntriesAlreadyDownloaded(wbe);

    if (!is_already_downloaded) {
        char* url = WBConfigForgeDownloadURL(wbe, wbcred);
        GetEbook(url, filename);
        free(url);
    }

    free(filename);
}


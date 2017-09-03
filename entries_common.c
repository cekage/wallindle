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

#include <time.h>
#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include "shared.h"
#include "configmanager.h"
#include "entries_common.h"

char* WBEntryFetchingURL(WBoAuthCred* wbc) {

    char* url;
    int last_week_in_utc;
    int sprinted;

    // TODO(k) Check real size instead of MAX_INT_STRLEN
    const  size_t  url_size = ( sizeof(FETCH_ENTRIES_MASK) - 1 //themasksize1
                                - (2 * ( sizeof("%s") - 1) + ( sizeof("%d") - 1)) //minusformats
                                + MAX_INT_STRLEN//plusmaxintsize
                                + strlen(wbc->wallabag_host)
                                + strlen(wbc->token) + 1
                              ) * sizeof(char);

    url = calloc(url_size + 1, sizeof(char));

    if (NULL == url) {
        fprintf(stderr, "Cannot allocate fetching url");
        return NULL;
    }

    last_week_in_utc = time(NULL) - UP_TO_X_DAYS * 24 * 60;
    sprinted = snprintf(url, url_size, FETCH_ENTRIES_MASK,
                        wbc->wallabag_host, wbc->token, last_week_in_utc);

    if (1 > sprinted) {
        fprintf(stderr, "Cannot snprintf fetching url");
        free(url);
        return NULL;
    }

    return url;
}

void WBEntryCleaup(WBEntry* wbe) {
    free(wbe->created_at);
    wbe->created_at = NULL;
    wbe->id = 0;
}

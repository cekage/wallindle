#include <time.h>
#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include "shared.h"
#include "configmanager.h"
#include "entries_common.h"

char* WBEntryFetchingURL(WBoAuthCred* wbc) {
    const int last_week_in_utc = time(NULL) - 70 * 24 * 60;

// TODO(k) Check real size
    const  size_t  url_size = ( sizeof(FETCH_ENTRIES_MASK) - 1 //themasksize1
                                - (2 * ( sizeof("%s") - 1) + ( sizeof("%d") - 1)) //minusformats
                                + MAX_INT_STRLEN//plusmaxintsize
                                + strlen(wbc->wallabag_host)
                                + strlen(wbc->token) + 1
                              ) * sizeof(char);
    printf("url_size=%" PRIuPTR, url_size);

    char* url = calloc(url_size  + 1, sizeof(char));
    printf("\nAvant: %" PRIuPTR "%s\n", url_size, url);
    snprintf(url, url_size, FETCH_ENTRIES_MASK,
             wbc->wallabag_host, wbc->token, last_week_in_utc);
    printf("\nAprès: %" PRIuPTR "%s\n", url_size, url);
    return url;
}

#ifndef SHARED_H_INCLUDED
#define SHARED_H_INCLUDED

#include <sys/param.h>

#ifdef __arm__
#define DBUS_CMD "/usr/bin/dbus-send"
#else
#define DBUS_CMD "/bin/echo"
#endif

#define DBUS_ARGS "--system/defaultcom.lab126.powerd.resumingint32:1"

//TODO(k)ugly!
#define MAX_INT_STRLEN (sizeof("4294967295"))

#define WNDL_ERROR (-1)
#define WNDL_OK (0)

#define LIMIT_DYNAMIC_STRING (4096)

#define MIN_CONFFILE_SIZE (50)
#define MAX_CONFFILE_SIZE (4096)

#define AUTH_URL_MASK "http://%s/oauth/v2/token?grant_type=password&client_id=%s&client_secret=%s&username=%s&password=%s"

#define FORMAT_EXPORT "mobi"

#ifdef __arm__
#define ENTRY_MASK "/mnt/base-us/documents/wallabag/%08lx."FORMAT_EXPORT
#else
#define ENTRY_MASK "./entries/%08lx."FORMAT_EXPORT
#endif

#define DOWNLOAD_URL_MASK "http://%s/api/entries/%lu/export."FORMAT_EXPORT"?access_token=%s"
#define FETCH_ENTRIES_MASK "http://%s/api/entries.json?access_token=%s&perPage=500&page=1&since=%d&sort=created"

#define DEFAULT_CONFIG_FILE "./wallindle.cfg"

typedef enum {false, true} bool ;


static int StoreContent(const char* src, size_t contentsize, char** dest) {
    const  size_t  realsize = MIN(contentsize, LIMIT_DYNAMIC_STRING);

// TODO(k) find which part transmit an uninitialized dest!
    if (NULL == *dest) {
        *dest = calloc((realsize + 1), sizeof(char));
    } else {
        *dest = realloc(*dest, (realsize + 1) * sizeof(char));
    }

    if (NULL == *dest) {
        fprintf(stderr, "%s", "not enough memory (realloc returned NULL)\n");
        return WNDL_ERROR;
    }

    strncpy(*dest, src, realsize);
    *(*dest + realsize) = 0;
    return WNDL_OK;
}

#endif//SHARED_H_INCLUDED

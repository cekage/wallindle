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

#ifndef SHARED_H_INCLUDED
#define SHARED_H_INCLUDED

#include <stdbool.h>

#ifdef __arm__
#define DBUS_CMD "/usr/bin/dbus-send"
#else
#define DBUS_CMD "/bin/echo"
#endif

#define DBUS_ARGS "--system","/default","com.lab126.powerd.resuming","int32:1"

//TODO(k) Ugly!
#define MAX_INT_STRLEN (sizeof("4294967295"))

//#define WNDL_ERROR (-1)
//#define WNDL_OK (0)

#define LIMIT_DYNAMIC_STRING (4096)

#define MIN_CONFFILE_SIZE (50)
#define MAX_CONFFILE_SIZE (4096)

#define AUTH_URL_MASK "http://%s/oauth/v2/token?grant_type=password&client_id=%s&client_secret=%s&username=%s&password=%s"

#define FORMAT_EXPORT "mobi"

#ifdef __arm__
#define ENTRY_MASK_DIR "/mnt/base-us/documents/wallabag"
#else
#define ENTRY_MASK_DIR "./entries"
#endif

#define ENTRY_MASK ENTRY_MASK_DIR "/%08lx." FORMAT_EXPORT

#define MAXIMUM_ENTRIES (500)
#define SMAXIMUM_ENTRIES "500"

#define DOWNLOAD_URL_MASK "http://%s/api/entries/%lu/export." FORMAT_EXPORT "?access_token=%s"
//#define FETCH_ENTRIES_MASK "http://%s/api/entries.json?access_token=%s&perPage=" SMAXIMUM_ENTRIES "&page=1&since=%d&sort=created"
#define FETCH_ENTRIES_MASK "http://%s/api/entries.json?access_token=%s&perPage=" SMAXIMUM_ENTRIES "&page=1&since=%d"

#define DEFAULT_CONFIG_FILE "./wallindle.cfg"

#define UP_TO_X_DAYS (70)

#define WALLINDLE_USERAGENT ("agent-ckg-fait-mumuse-avec-libcurl/1.0")

typedef enum wd_result {
    WNDL_OK,
    WNDL_ERROR
} wd_result;


wd_result StoreContent(const char* src, size_t contentsize, char** dest);

void EnsureEbookDirExists(void);
#endif//SHARED_H_INCLUDED

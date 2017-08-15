#ifndef SHARED_H_INCLUDED
#define SHARED_H_INCLUDED

#ifdef __arm__
#define DBUS_CMD "/usr/bin/dbus-send"
#else
#define DBUS_CMD "/bin/echo"
#endif

#define DBUS_ARGS "--system","/default","com.lab126.powerd.resuming","int32:1"

//TODO(k)ugly!
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
#define FETCH_ENTRIES_MASK "http://%s/api/entries.json?access_token=%s&perPage=" SMAXIMUM_ENTRIES "&page=1&since=%d&sort=created"

#define DEFAULT_CONFIG_FILE "./wallindle.cfg"

// TODO(k) replace #define WNDL_OK
typedef enum {WNDL_OK, WNDL_ERROR} wd_result;

#include <stdbool.h>
//typedef enum {false, true} bool;

wd_result StoreContent(const char* src, size_t contentsize, char** dest);

void EnsureEbookDirExists(void);
#endif//SHARED_H_INCLUDED

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>

#include <sys/param.h>
#include <sys/types.h>

#include "shared.h"
#include "configmanager.h"

#include <sys/stat.h>
//
static wd_result _WBConfigGet(WBoAuthCred* wbcred, const char* cfg_filename);
static wd_result _WBReadConfiguration(const char* filename,
                                      off_t filesize, char** filecontent);
static void _WBConfigPrint(const WBoAuthCred* wbc);
static bool _WBConfigCompare(WBoAuthCred* wb1, WBoAuthCred* wb2);

static int WBConfigStringSet(const char* content, size_t  contentsize,
                             char** wbcfield);

off_t GetFileSize(const char* filename, bool check_min_max) {

    off_t result;
    struct stat fs_stat = {0};


    if (stat(filename, &fs_stat) != 0) {
        fprintf(stderr, "Cannot determine sizeof %s:%s\n", filename, strerror(errno));
        result = -1;
    } else if (check_min_max && (MIN_CONFFILE_SIZE > fs_stat.st_size
                                 || MAX_CONFFILE_SIZE < fs_stat.st_size)) {
        fprintf(stderr,
                "Suspiscious size for %s : MIN_CONFFILE_SIZE=%d MAX_CONFFILE_SIZE=%d\n",
                filename, MIN_CONFFILE_SIZE, MAX_CONFFILE_SIZE);
        result = -1;
    } else {
        result = fs_stat.st_size;
    }

    return result;
}

wd_result _WBReadConfiguration(const char* filename,
                               off_t filesize, char** filecontent) {
    size_t  totalread;
    FILE* f = fopen(filename, "r");

    if (NULL == f) {
        fprintf(stderr, "Cannot open %s:%s\n", filename, strerror(errno));
        return WNDL_ERROR;
    }

    totalread = fread(*filecontent, sizeof(char),
                      (size_t)filesize, f);
    fclose(f);

    if ( sizeof(char) > totalread) {
        fprintf(stderr, "Cannot determine sizeof %s:%s\n", filename,
                strerror(errno));
        return WNDL_ERROR;
    }

    return WNDL_OK;
}

wd_result WBConfigGet(WBoAuthCred* wbcred) {
    return _WBConfigGet(wbcred, DEFAULT_CONFIG_FILE);
}

static void _AuthFieldFoundAndSave(bool* proceed, char* src, char** dest,
                                   const char* fieldname) {
    const char* delim = " ";
    char* token;
    bool result;

    if (proceed) {
        token = strtok(src, delim);
        result = (NULL != token);

        if (result) {
            size_t  last_char_pos = strlen(token) - 1;

            if (0x0A == token[last_char_pos]) {
                token[last_char_pos] = 0;
            }

            WBConfigStringSet(token, strlen(token), dest);
        } else {
            fprintf(stderr, "Cannot determine field %s\n", fieldname);
        }

        *proceed = result;
    }

}

static wd_result _WBConfigGet(WBoAuthCred* wbcred, const char* cfg_filename) {

    off_t filesize;
    char* filecontent;
    int readresult;
    bool must_continue = true;


    filesize = GetFileSize(cfg_filename, true);

    if (-1 == filesize) {
        return WNDL_ERROR;
    }

    filecontent = calloc(filesize + 1UL, sizeof(char));

    if (NULL == filecontent) {
        return WNDL_ERROR;
    }

    if (must_continue) {
        readresult = _WBReadConfiguration(cfg_filename, filesize, &filecontent);
        must_continue = (WNDL_OK == readresult);
        _AuthFieldFoundAndSave(&must_continue, filecontent, &wbcred->wallabag_host,
                               "wallabag_host");
        _AuthFieldFoundAndSave(&must_continue, NULL, &wbcred->client_id, "client_id");
        _AuthFieldFoundAndSave(&must_continue, NULL, &wbcred->client_secret,
                               "client_secret");
        _AuthFieldFoundAndSave(&must_continue, NULL, &wbcred->username, "username");
        _AuthFieldFoundAndSave(&must_continue, NULL, &wbcred->password, "password");

    }

    free(filecontent);

    return must_continue ? WNDL_OK : WNDL_ERROR;
}

// TODO(k) Assign default null values, avoid useless calloc();
wd_result WBConfigInit(WBoAuthCred* wbc) {
#define CHECKFIELD(FD) result &= (NULL!=wbc->FD);
#define INITFIELD(FD) wbc->FD=calloc(1, sizeof(char)); CHECKFIELD(FD)
    bool result = true;
    INITFIELD(wallabag_host);
    INITFIELD(client_id);
    INITFIELD(client_secret);
    INITFIELD(username);
    INITFIELD(password);
    INITFIELD(token);
    return result ? WNDL_OK : WNDL_ERROR ;
#undef  INITFIELD
#undef  CHECKFIELD
}

void WBConfigCleanup(WBoAuthCred* wbc) {
#define FREEFIELD(FD) free(wbc->FD);wbc->FD=NULL;
    FREEFIELD(wallabag_host);
    FREEFIELD(client_id);
    FREEFIELD(client_secret);
    FREEFIELD(username);
    FREEFIELD(password);
    FREEFIELD(token);
#undef  FREEFIELD
}

static int WBConfigStringSet(const char* content, size_t  contentsize,
                             char** wbcfield) {

    return StoreContent(content, contentsize, wbcfield);
}

char* WBConfigForgeoAuthURL(WBoAuthCred* wbc) {

    char* url;
    size_t url_size;

    // Calculate size of url using MASK minus 5* "%s"
    // plus the length of wbs fields
    url_size = ( sizeof(AUTH_URL_MASK) - 5 * ( sizeof("%s") - 1)
                 + strlen(wbc->wallabag_host) + strlen(wbc->client_id)
                 + strlen(wbc->client_secret) + strlen(wbc->username)
                 + strlen(wbc->password)) * sizeof(char);

    // Allocate with zeros
    url = calloc(url_size + 1, sizeof(char));

    if (NULL == url) {
        // If fails, warns user on stderr
        fprintf(stderr, "Cannot allocate oauth url\n");
    } else {
        // Store in "url" the mask and parameters
        int sprinted = snprintf(url, url_size, AUTH_URL_MASK, wbc->wallabag_host,
                                wbc->client_id, wbc->client_secret, wbc->username,
                                wbc->password);

        // Checking if snprintf works as expected (size > 1)
        if (1 > sprinted) {
            // If fails : warns, frees & nulls url
            fprintf(stderr, "Cannot snprintf oauth url\n");
            free(url);
            url = NULL;
        }
    }

    return url;
}

static void _WBConfigPrint(const WBoAuthCred* wbc) {
#define PRINTFIELD(FD) printf("wbc->%s(%"PRIuPTR"o.)=%s\n",#FD,strlen(wbc->FD),wbc->FD)
    PRINTFIELD(wallabag_host);
    PRINTFIELD(client_id);
    PRINTFIELD(client_secret);
    PRINTFIELD(username);
    PRINTFIELD(password);
    PRINTFIELD(token);
#undef  PRINTFIELD
}

static bool _WBConfigCompare(WBoAuthCred* wb1, WBoAuthCred* wb2) {
#define CHECKFIELD(FD) result &=(0==strcmp(wb1->FD,wb2->FD))
    bool  result = true;
    CHECKFIELD(wallabag_host);
    CHECKFIELD(client_id);
    CHECKFIELD(client_secret);
    CHECKFIELD(username);
    CHECKFIELD(password);
    CHECKFIELD(token);
    return result;
#undef  CHECKFIELD
}


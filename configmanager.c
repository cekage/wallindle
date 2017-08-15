#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <errno.h>


#include <inttypes.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "shared.h"

#include "configmanager.h"

static wd_result _WBConfigGet(WBoAuthCred* wbcred, const char* cfg_filename);
static wd_result _WBReadConfiguration(const char* filename,
                                      off_t filesize, char** filecontent);
static void _WBConfigPrint(const WBoAuthCred* wbc);
static bool _WBConfigCompare(WBoAuthCred* wb1, WBoAuthCred* wb2);


off_t CheckConfSize(const char* filename, bool check_min_max) {
    struct stat fs_stat;

    off_t result;

    if (stat(filename, &fs_stat) != 0) {
        fprintf(stderr, "Cannot determine sizeof %s:%s\n", filename, strerror(errno));
        result = -1;
    }

    else if (check_min_max && (MIN_CONFFILE_SIZE > fs_stat.st_size
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
        fprintf(stderr, "Cannot determine sizeof %s:%s\n",               filename,
                strerror(errno));
        return WNDL_ERROR;
    }

    return WNDL_OK;
}

wd_result WBConfigGet(WBoAuthCred* wbcred) {
    return _WBConfigGet(wbcred, DEFAULT_CONFIG_FILE);
}

static wd_result _WBConfigGet(WBoAuthCred* wbcred, const char* cfg_filename) {
    const char* delim = " ";

    off_t filesize;
    char* filecontent;
    int readresult;
    char* token;

    filesize = CheckConfSize(cfg_filename, true);

    if (-1 == filesize) {
        return WNDL_ERROR;
    }


    filecontent = calloc((filesize + 1UL), sizeof(char));

    if (NULL == filecontent) {
        return WNDL_ERROR;
    }

    readresult = _WBReadConfiguration(cfg_filename, filesize, &filecontent);

    if (WNDL_ERROR == readresult) {
        free(filecontent);
        return WNDL_ERROR;
    }

//printf("%"PRIuPTR"%s\n",filesize,filecontent);
//   WBConfigPrint(wbcred);

    token = strtok(filecontent, delim);

    if (NULL != token) {
        WBConfigStringSet(token, strlen(token), &wbcred->wallabag_host);
    } else {
        fprintf(stderr, "Cannot determine field %s\n", "wallabag_host");
        free(filecontent);
        return WNDL_ERROR;
    }

    token = strtok(NULL, delim);

    if (NULL != token) {
        WBConfigStringSet(token, strlen(token), &wbcred->client_id);
    } else {
        fprintf(stderr, "Cannot determine field %s\n", "client_id");
        free(filecontent);
        return WNDL_ERROR;
    }

    token = strtok(NULL, delim);

    if (NULL != token) {
        WBConfigStringSet(token, strlen(token), &wbcred->client_secret);
    } else {
        fprintf(stderr, "Cannot determine field %s\n", "client_secret");
        free(filecontent);
        return WNDL_ERROR;
    }

    token = strtok(NULL, delim);

    if (NULL != token) {
        WBConfigStringSet(token, strlen(
                              token), &wbcred->username);
    } else {
        fprintf(stderr, "Cannot determine field %s\n", "username");
        free(filecontent);
        return WNDL_ERROR;
    }

    token = strtok(NULL, delim);

    if (NULL != token) {
        size_t  last_char_pos = strlen(token) - 1;

        if (0x0A == token[last_char_pos]) {
            token[last_char_pos] = 0;
        }

        WBConfigStringSet(token, strlen(token), &wbcred->password);
    } else {
        fprintf(stderr, "Cannot determine field %s\n", "password");
        free(filecontent);
        return WNDL_ERROR;
    }

    free(filecontent);
//WBConfigPrint(wbcred);
    return WNDL_OK;
}

// TODO(k) Assign default null values : https://stackoverflow.com/a/749690
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

int WBConfigStringSet(const char* content,  size_t  contentsize,
                      char** wbcfield) {
    return StoreContent(content, contentsize, wbcfield);
}




char* WBConfigForgeoAuthURL(WBoAuthCred* wbc) {
    const  size_t  url_size = ( sizeof(AUTH_URL_MASK)
                                - 5 * ( sizeof("%s") - 1)
                                + strlen(wbc->wallabag_host)
                                + strlen(wbc->client_id)
                                + strlen(wbc->client_secret)
                                + strlen(wbc->username)
                                + strlen(wbc->password)
                              ) * sizeof(char);
    char* url = calloc(url_size + 1, sizeof(char));

    if (NULL != url)
    { snprintf(url, url_size, AUTH_URL_MASK, wbc->wallabag_host, wbc->client_id, wbc->client_secret, wbc->username, wbc->password); }

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


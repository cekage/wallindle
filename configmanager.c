#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <errno.h>

//#ifndef CONFIGMANAGER_H
#include "configmanager.h"
//#endif

#include <inttypes.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "shared.h"

off_t CheckConfSize(const char* filename) {
    struct stat st;

    off_t result;

    if (stat(filename, &st) != 0) {
        fprintf(stderr, "Cannot determine sizeof %s:%s\n", filename, strerror(errno));
        result = WNDL_ERROR;
    }

    else if (MIN_CONFFILE_SIZE > st.st_size || MAX_CONFFILE_SIZE < st.st_size) {
        fprintf(stderr,
                "Suspiscious size for %s : MIN_CONFFILE_SIZE=%d MAX_CONFFILE_SIZE=%d\n",
                filename, MIN_CONFFILE_SIZE, MAX_CONFFILE_SIZE);
        result = WNDL_ERROR;
    } else {
        result = st.st_size;
    }

    return result;
}

int _WBReadConfiguration(const char* filename,
                                off_t filesize, char** filecontent) {
    FILE* f = fopen(filename, "r");

    if (NULL == f) {
        fprintf(stderr, "Cannot open %s:%s\n", filename, strerror(errno));
        return WNDL_ERROR;
    }

    const  size_t  totalread = fread(*filecontent, sizeof(char),
                                     (size_t)filesize, f);
    fclose(f);

    if ( sizeof(char) > totalread) {
        fprintf(stderr, "Cannot determine sizeof %s:%s\n",               filename,
                strerror(errno));
        return WNDL_ERROR;
    }

    return WNDL_OK;
}

int WBConfigGet(WBoAuthCred* wbcred) {
    return _WBConfigGet(wbcred, DEFAULT_CONFIG_FILE);
}

int _WBConfigGet(WBoAuthCred* wbcred, const char* cfg_filename) {
    const char* delim = " ";

    off_t filesize;
    char* filecontent;
    int readresult;
    char* token;

    filesize = CheckConfSize(cfg_filename);

    if (WNDL_ERROR == filesize) {
        return WNDL_ERROR;
    }


    filecontent = calloc((unsigned long)filesize + 1UL,
                         sizeof(char));

    if (NULL == filecontent) {
        return WNDL_ERROR;
    }

    readresult = _WBReadConfiguration(cfg_filename, filesize, &filecontent);

    if (WNDL_ERROR == readresult) {
        free(filecontent);
        return WNDL_ERROR;
    }

//printf("%"PRIuPTR"%s\n",filesize,filecontent);
//    WBConfigPrint(wbcred);

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
int WBConfigInit(WBoAuthCred* wbc) {
#define CHECKFIELD(FD) result &= (NULL!=wbc->FD);
#define INITFIELD(FD) wbc->FD=calloc(1, sizeof(char)); CHECKFIELD(FD)
    bool  result = WNDL_OK;
    INITFIELD(wallabag_host);
    INITFIELD(client_id);
    INITFIELD(client_secret);
    INITFIELD(username);
    INITFIELD(password);
    INITFIELD(token);
    return result;
#undef  INITFIELD
#undef  CHECKFIELD
}

void  WBConfigCleanup(WBoAuthCred* wbc) {
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

void  WBConfigPrint(const WBoAuthCred* wbc) {
#define PRINTFIELD(FD) printf("wbc->%s(%"PRIuPTR"o.)=%s\n",#FD,strlen(wbc->FD),wbc->FD)
    PRINTFIELD(wallabag_host);
    PRINTFIELD(client_id);
    PRINTFIELD(client_secret);
    PRINTFIELD(username);
    PRINTFIELD(password);
    PRINTFIELD(token);
#undef  PRINTFIELD
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
    //printf("\n*url_size=%"PRIuPTR"and sizeof(\"%%s\")%d",url_size, sizeof("%s"));
    char* url = calloc(url_size + 1, sizeof(char));
    //printf("\nAvant:%"PRIuPTR"\"%s\"--<\n",url_size,url);
    snprintf(url, url_size, AUTH_URL_MASK, wbc->wallabag_host, wbc->client_id, wbc->client_secret, wbc->username, wbc->password);
    //printf("\nAprès:%"PRIuPTR"%s\n",url_size,url);
    return url;
}

bool  WBConfigCompare(WBoAuthCred* wb1, WBoAuthCred* wb2) {
#define CHECKFIELD(FD) (0==strcmp(wb1->FD,wb2->FD))
    bool  result;
    result = CHECKFIELD(wallabag_host);
    result &= CHECKFIELD(client_id);
    result &= CHECKFIELD(client_secret);
    result &= CHECKFIELD(username);
    result &= CHECKFIELD(password);
    result &= CHECKFIELD(token);
    return result;
#undef  CHECKFIELD
}


#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "http_request.h"
#include "json_entries_parse.h"
#include "configmanager.h"
#include "entries_parse.h"
#include "perform_entries.h"
#include "entries_common.h"
#include "json_oauth_parse.h"

#include "shared.h"

static void UpdateKindleCatalog(void );

static void UpdateKindleCatalog(void ) {
    __pid_t pid = fork();

    if (pid == -1) {
        fprintf(stderr, "Can't fork\n");
    } else if (pid > 0) {
        int status;

        if (-1 == waitpid(pid, &status, 0)) {
            printf("Error for waitpid :(\n");
        }
    } else {
        /*@null@*/
        char* command = DBUS_CMD;
        char* arguments[] = {"", DBUS_ARGS, NULL};

        if (-1 == execvp(command, arguments)) {
            printf("Error for execvp :(\n");
        }

        exit(EXIT_FAILURE);
    }
}

int main(void ) {
    char* oauthurl;
    char* getentriesurl;
    char* filename;
    char* ebookurl;
    WBEntry* entries;
    int i;

    MemoryStruct entriesjsonresponse = (MemoryStruct) {NULL, 0};
    WBoAuthCred a_wbc = (WBoAuthCred) {NULL, NULL, NULL, NULL, NULL, NULL};
    MemoryStruct authjsonresponse = (MemoryStruct) {NULL, 0};

    if (WNDL_ERROR == WBConfigInit(&a_wbc)) {
        exit(EXIT_FAILURE);
    }

    if (WNDL_ERROR == WBConfigGet(&a_wbc)) {
        exit(EXIT_FAILURE);
    }

    oauthurl = WBConfigForgeoAuthURL(&a_wbc);

    if (NULL == oauthurl) {
        exit(EXIT_FAILURE);
    }

//    authjsonresponse.memory = NULL;
//    authjsonresponse.size = 0;


    if (WNDL_ERROR == GetJSON(oauthurl, &authjsonresponse)) {
        free(oauthurl); // Avoiding valgrind warnings
        exit(EXIT_FAILURE);
    }

    free(oauthurl);

    a_wbc.token = ExtractToken(authjsonresponse.memory);

    if (NULL == a_wbc.token) {
        WBConfigCleanup(&a_wbc);
        free(authjsonresponse.memory);
        exit(EXIT_FAILURE);
    }

    free(authjsonresponse.memory);

    getentriesurl = WBEntryFetchingURL(&a_wbc);

    if (WNDL_ERROR == GetJSON(getentriesurl, &entriesjsonresponse)) {
        free(getentriesurl); // Avoiding valgrind warnings
        exit(EXIT_FAILURE);
    }

    free(getentriesurl);

    printf("\nentriesjsonresponse.memory = \n%s\n", entriesjsonresponse.memory);

    entries = JsonGetEntries(entriesjsonresponse.memory);
    free(entriesjsonresponse.memory);

    EnsureEbookDirExists();

    for (i = 0; (i < MAXIMUM_ENTRIES) && (0 != entries[i].id); ++i) {

        _PrintEntry(&entries[i]);
        filename = GetEntryFileName(&entries[i]);

        if (!IsEbookAlreadyDownloaded(filename)) {
            ebookurl = WBConfigForgeDownloadURL(&entries[i], &a_wbc);

            if (WNDL_ERROR == GetEbook(ebookurl, filename)) {
                printf("fetching %s failed\n", ebookurl);
            };

            free(ebookurl);

            ebookurl = NULL;
        } else {
            printf("%s already downloaded !\n", filename);
        }

        free(filename);
        filename = NULL;
        free(entries[i].created_at);
        entries[i].created_at = NULL;
    }

    free(entries);
    WBConfigCleanup(&a_wbc);

    UpdateKindleCatalog();
    printf("MischiefManaged!\n");
    return 0;
}

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

#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "http_request.h"
#include "json_entries_parse.h"
#include "configmanager.h"
//#include "entries_parse.h"
#include "perform_entries.h"
#include "entries_common.h"
#include "json_oauth_parse.h"

#include "shared.h"

static void UpdateKindleCatalog(void ) {
    pid_t pid = fork();

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
        char* arguments[] = {DBUS_CMD, DBUS_ARGS, NULL};

        if (-1 == execvp(command, arguments)) {
            printf("Error for execvp :(\n");
        }

        exit(EXIT_FAILURE);
    }
}

static void ProceedUpdate(WBEntry* entries, WBoAuthCred a_wbc) {
    unsigned int i;

    // Iterate entries until MAXIMUM or the first time id is zero
    for (i = 0; (i < MAXIMUM_ENTRIES) && (0 != entries[i].id); ++i) {

        char* ebookurl;
        // Retrieve file name
        char* filename = GetEntryFileName(&entries[i]);

        // Check if filename is null, must not happened !
        if (NULL == filename) {
            // in this case, go to next entries
            continue;
        }

//        // Check if ebook already exists
//        if (IsEbookAlreadyDownloaded(filename)) {
//            // in this case, free stuff and go to next entries
//            free(filename);
//            continue;
//        }

        printf("Downloading item %lu -> %s \n", entries[i].id, filename);

        // Compute Url for downloading ebook
        ebookurl = WBConfigForgeDownloadURL(&entries[i], &a_wbc);

        // If getting ebook raises an error
        if (WNDL_ERROR == GetEbook(ebookurl, filename)) {
            // then displays error message
            printf("fetching %s failed\n", ebookurl);
        };

        free(ebookurl);
        free(filename);

    }
}

// TODO(k) Too long !!
int main(void ) {
    char* oauthurl;
    char* getentriesurl;
    WBEntry* entries;
    WBoAuthCred a_wbc;

    // Init auth json with empty values
    MemoryStruct authjsonresponse = (MemoryStruct) {NULL, 0};
    // Init entries json with empty values
    MemoryStruct entriesjsonresponse = (MemoryStruct) {NULL, 0};

    // Initialize configuration
    if (WNDL_ERROR == WBConfigInit(&a_wbc)) {
        WBConfigCleanup(&a_wbc);
        exit(EXIT_FAILURE);
    }

    // Retrieve config data from config file
    if (WNDL_ERROR == WBConfigGet(&a_wbc)) {
        WBConfigCleanup(&a_wbc);
        exit(EXIT_FAILURE);
    }

    // Retrieve the forged url to get auth token
    oauthurl = WBConfigForgeoAuthURL(&a_wbc);

    //    for debug :
    //    printf("\noauthurl = \n%s\n", oauthurl);

    // Check if oAuth2 url is correctly build
    if (NULL == oauthurl) {
        WBConfigCleanup(&a_wbc);
        exit(EXIT_FAILURE);
    }

    // Check if oAuth2 url gives a json response
    if (WNDL_ERROR == GetJSON(oauthurl, &authjsonresponse)) {
        free(oauthurl); // Avoiding valgrind warnings
        WBConfigCleanup(&a_wbc);
        exit(EXIT_FAILURE);
    }

    // TODO(k) maybe another way than doubling free !
    free(oauthurl);

    // TODO(k) Avoiding memory leak…
    free(a_wbc.token);
    // Store the token in the cred object
    a_wbc.token = ExtractoAuth2Token(authjsonresponse.memory);

    // No token ?
    if (NULL == a_wbc.token) {
        // Freeing cred object
        WBConfigCleanup(&a_wbc);
        // Freeing memory
        free(authjsonresponse.memory);
        exit(EXIT_FAILURE);
    }

    // TODO(k) maybe another way than doubling free !
    free(authjsonresponse.memory);

    getentriesurl = WBEntryFetchingURL(&a_wbc);

    if (WNDL_ERROR == GetJSON(getentriesurl, &entriesjsonresponse)) {
        free(getentriesurl); // Avoiding valgrind warnings
        exit(EXIT_FAILURE);
    }

    // TODO(k) maybe another way than doubling free !
    free(getentriesurl);

    //    for debug :
    //    printf("\nentriesjsonresponse.memory = \n%s\n", entriesjsonresponse.memory);

    entries = JsonGetEntries(entriesjsonresponse.memory);
    free(entriesjsonresponse.memory);

    if (NULL != entries) {
        unsigned int i;
        EnsureEbookDirExists();
        ProceedUpdate(entries, a_wbc);

        for (i = 0; (i < MAXIMUM_ENTRIES) && (0 != entries[i].id); ++i) {
            WBEntryCleaup(&entries[i]);
        }

        free(entries);
        UpdateKindleCatalog();
    }

    WBConfigCleanup(&a_wbc);

    printf("\nMischief Managed!\n");
    return 0;
}



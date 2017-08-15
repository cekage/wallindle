#include <unistd.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "http_request.h"
#include "json_oauth_parse.h"
#include "json_entries_parse.h"
#include "configmanager.h"
#include "entries_parse.h"
#include "perform_entries.h"
#include "entries_common.h"

#include "shared.h"

void UpdateKindleCatalog(void );

void UpdateKindleCatalog(void ) {
    pid_t pid = fork();

    if (pid == -1) {
        fprintf(stderr, "Can't fork\n");
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
    } else {
        char* command = DBUS_CMD;
        char* arguments[] = {"", DBUS_ARGS, NULL};
        execvp(command, arguments);
        _exit(EXIT_FAILURE);
    }
}

int main(void ) {
    WBoAuthCred a_wbc;
    WBConfigInit(&a_wbc);
    int retourWD = WBConfigGet(&a_wbc);

    char* oauthurl;
    oauthurl = WBConfigForgeoAuthURL(&a_wbc);

    MemoryStruct authjsonresponse = (MemoryStruct) {.memory = malloc(1), .size = 0};
    GetJSON(oauthurl, &authjsonresponse);
    free(oauthurl);

    a_wbc.token = ExtractToken(authjsonresponse.memory);
    free(authjsonresponse.memory);

    char* getentriesurl;
    getentriesurl = WBEntryFetchingURL(&a_wbc);

    MemoryStruct entriesjsonresponse = (MemoryStruct) {.memory = calloc(100, sizeof(char)), .size = 0};
    GetJSON(getentriesurl, &entriesjsonresponse);
    free(getentriesurl);

    printf("\nentriesjsonresponse.memory = \n%s\n", entriesjsonresponse.memory);

    WBEntry* entries = JsonGetEntries(entriesjsonresponse.memory);
    free(entriesjsonresponse.memory);

    EnsureEbookDirExists();
    for (int i = 0; i < MAXIMUM_ENTRIES; ++i) {
        if (0 == entries[i].id) { break; }

        _PrintEntry(&entries[i]);
        char* filename = GetEntryFileName(&entries[i]);

        if (!IsEbookAlreadyDownloaded(filename)) {
            char* ebookurl = WBConfigForgeDownloadURL(&entries[i], &a_wbc);
            printf("ebookurl = %s\n", ebookurl);
            GetEbook(ebookurl, filename);
            free(ebookurl);
        } else {
            printf("%s already downloaded !\n", filename);
        }

        free(filename);
        free(entries[i].created_at);
    }

    free(entries);
    WBConfigCleanup(&a_wbc);

    UpdateKindleCatalog();
    printf("MischiefManaged!\n");
    return 0;
}

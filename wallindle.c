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
#include "entries_common.h"

#include "shared.h"

WBoAuthCred a_wbc;

void PerformAuth(void );
void PerformReal(void );
void PerformDummy(void );
void FetchEntries(void );
void UpdateKindleCatalog(void );

void PerformAuth(void ) {
    char* url;
    MemoryStruct jsonresponse;

    WBConfigGet(&a_wbc);
    WBConfigPrint(&a_wbc);

    url = WBConfigForgeoAuthURL(&a_wbc);
    jsonresponse.memory = malloc(1);
    jsonresponse.size = 0;

    GetJSON(url, &jsonresponse);
    free(url);

    printf("\nLa réponse json est:%s\n", jsonresponse.memory);
    a_wbc.token = ExtractToken(jsonresponse.memory);
    WBConfigPrint(&a_wbc);

    free(jsonresponse.memory);
}

void FetchEntries(void ) {
    char* url;
    MemoryStruct jsonresponse;

    url = WBEntryFetchingURL(&a_wbc);
    jsonresponse = (MemoryStruct) {calloc(1, sizeof(char)), 0};

//TODO(k)return achar*
    GetJSON(url, &jsonresponse);
    free(url);

    printf("\nLa réponse json est:\n**** DEBUT %s \n****FIN\n",
           jsonresponse.memory);
    WBReadDownloadedJsonEntries(jsonresponse.memory, &a_wbc);
    free(jsonresponse.memory);
}

void PerformDummy(void ) {
    StoreContent("wallabag.thouzard.fr", 30, &a_wbc.wallabag_host);
    StoreContent("NjkwMGYyNTQ0ZWEyMzM1MGFlNGM5MWVkYzY3NTJkNDE5NjExOWIxYjAxODZkMDNlYWU0MDRjNmI3YjI0NmFkZg",
                 100, &a_wbc.token);
//WBReadJsonEntries(&a_wbc);
    FetchEntries();
}

void PerformReal(void ) {
    PerformAuth();
    FetchEntries();
//WBReadDownloadedJsonEntries()
}

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
    WBConfigInit(&a_wbc);
    PerformReal();
//PerformDummy();
    WBConfigCleanup(&a_wbc);

    UpdateKindleCatalog();
    printf("MischiefManaged!\n");
    return 0;
}

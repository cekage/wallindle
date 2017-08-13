#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>

// TODO(k)Assigndefaultnullvalueshttps://stackoverflow.com/a/749690
static int WBoAuth2Init(WBoAuth2Struct* wbo) {
#define CHECKFIELD(FD) result &= (NULL != wbo->FD);
#define INITFIELD(FD) wbo->FD = calloc(1, sizeof(char));CHECKFIELD(FD)
    bool result = WNDL_OK;
    INITFIELD(access_token);
    INITFIELD(refresh_token);
    INITFIELD(scope);
    INITFIELD(token_type);
    wbo->expires_in = 0;
    return result;
#undef INITFIELD
#undef CHECKFIELD
}

static void WBoAuth2Cleanup(WBoAuth2Struct* wbo) {
#define FREEFIELD(FD) free(wbo->FD);wbo->FD = NULL;
    FREEFIELD(access_token);
    FREEFIELD(refresh_token);
    FREEFIELD(scope);
    FREEFIELD(token_type);
    wbo->expires_in = 0;
#undef FREEFIELD
}

static void WBoAuth2Print(const WBoAuth2Struct* wbo) {
#define PRINTFIELD(FD) printf("wbo->%s(%" PRIuPTR "o.)=%s\n", #FD, strlen(wbo->FD), wbo->FD)
    printf("\n");
    PRINTFIELD(access_token);
    PRINTFIELD(refresh_token);
    PRINTFIELD(scope);
    PRINTFIELD(token_type);
    printf("wbo->token_type=%u\n", wbo->expires_in);
#undef PRINTFIELD
}

static int WBoAuth2StringSet(const char* content, size_t contentsize,
                             char** wbofield) {
    return StoreContent(content, contentsize, wbofield);
}

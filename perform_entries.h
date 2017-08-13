#ifndef PERFORM_ENTRIES_H_INCLUDED
#define PERFORM_ENTRIES_H_INCLUDED

#include "configmanager.h"

static bool  IsEntriesAlreadyDownloaded(const WBEntry* wbe);


static char* WBConfigForgeDownloadURL(const WBEntry* wbe,
                                      const WBoAuthCred* wbcred);

#include "perform_entries.c"

#endif//PERFORM_ENTRIES_H_INCLUDED

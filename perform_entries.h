#ifndef PERFORM_ENTRIES_H_INCLUDED
#define PERFORM_ENTRIES_H_INCLUDED

#include "configmanager.h"

bool  IsEntriesAlreadyDownloaded(const WBEntry* wbe);


char* WBConfigForgeDownloadURL(const WBEntry* wbe,
                                      const WBoAuthCred* wbcred);

char* GetEntryFileName(const WBEntry* wbe);

void PerformEverything(const WBEntry* wbe, const WBoAuthCred* wbcred);

#endif//PERFORM_ENTRIES_H_INCLUDED

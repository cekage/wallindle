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

#ifndef PERFORM_ENTRIES_H_INCLUDED
#define PERFORM_ENTRIES_H_INCLUDED

#include "configmanager.h"

// TODO(k) Add test
bool  IsEbookAlreadyDownloaded(const char* ebookfile);

char* WBConfigForgeDownloadURL(const WBEntry* wbe,
                               const WBoAuthCred* wbcred);

char* GetEntryFileName(const WBEntry* wbe);

#endif//PERFORM_ENTRIES_H_INCLUDED

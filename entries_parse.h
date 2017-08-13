#ifndef ENTRIES_PARSE_H_INCLUDED
#define ENTRIES_PARSE_H_INCLUDED

void  WBReadJsonEntries(WBoAuthCred* wbc, const char* filename);

void  WBReadDownloadedJsonEntries(const char* jsoncontent,
        WBoAuthCred* wbc);

#endif//ENTRIES_PARSE_H_INCLUDED

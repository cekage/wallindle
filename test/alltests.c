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

#include "greatest.h"

#include  "../configmanager.c"
#include  "../oauth_manager.c"
#include "../json_oauth_parse.c"
#include "../json_common.c"
#include "../json_entries_parse.c"
#include "../perform_entries.c"
#include "../http_request.c"
#include "../entries_common.c"

#include "_wbac_init.c"
#include "_wbac_compare.c"
#include "_wbac_setstring.c"
#include "_conffile.c"
#include "_wboa_init.c"
#include "_jsonauth.c"
#include "_jsonentries.c"
#include "_performentries.c"
#include "_entriescommon.c"
#include "_httprequest.c"

/* Add definitions that need to be in the test runner's main file. */
GREATEST_MAIN_DEFS();

int main(int argc, char** argv) {
    GREATEST_MAIN_BEGIN();      /* command-line arguments, initialization. */
    RUN_SUITE(_wbac_init);
    RUN_SUITE(_wbac_compare);
    RUN_SUITE(_wbac_setstring);
    RUN_SUITE(_conffile);
    RUN_SUITE(_wboa_init);
    RUN_SUITE(_jsonauth);
    RUN_SUITE(_jsonentries);
    RUN_SUITE(_performentries);
    RUN_SUITE(_entriescommon);
    RUN_SUITE(_httprequest);
    GREATEST_MAIN_END();        /* display results */
}

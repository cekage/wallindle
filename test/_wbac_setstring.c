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

#include  "../configmanager.h"

TEST wbac_setstring_ok() {
    WBoAuthCred known = (WBoAuthCred) {
        "exAmple.org", "10921029",
        "qlmksd,qcpsdqslkdqs,kdqjsmdl,qj",
        "mqimqsdicmqsx<sqidUOIUZKJSHQS",
        "toto123", ""
    };
    WBoAuthCred wac_default;
    WBConfigInit(&wac_default);

    WBConfigStringSet(known.wallabag_host, strlen(known.wallabag_host),
                      &wac_default.wallabag_host);
    WBConfigStringSet(known.client_id, strlen(known.client_id),
                      &wac_default.client_id);
    WBConfigStringSet(known.client_secret, strlen(known.client_secret),
                      &wac_default.client_secret);
    WBConfigStringSet(known.username, strlen(known.username),
                      &wac_default.username);
    WBConfigStringSet(known.password, strlen(known.password),
                      &wac_default.password);
    WBConfigStringSet(known.token, strlen(known.token),
                      &wac_default.token);

    bool  result1 = _WBConfigCompare(&known, &wac_default);
    bool  result2 = _WBConfigCompare(&wac_default, &known);
    WBConfigCleanup(&wac_default);

    ASSERT_EQ(true, result1);
    ASSERT_EQ(true, result2);
    PASS();
}

TEST wbac_print() {
    WBoAuthCred known = (WBoAuthCred) {
        "exAmple.org", "10921029",
        "qlmksd,qcpsdqslkdqs,kdqjsmdl,qj",
        "mqimqsdicmqsx<sqidUOIUZKJSHQS",
        "toto123", ""
    };
    _WBConfigPrint(&known);
    PASS();
}


SUITE(_wbac_setstring) {
    RUN_TEST(wbac_setstring_ok);
    RUN_TEST(wbac_print);
}

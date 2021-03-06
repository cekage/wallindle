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

TEST wbac_compare_ok() {
    WBoAuthCred known = (WBoAuthCred) {
        "example.org", "10921029",
        "qlmksd,qcpsdqslkdqs,kdqjsmdl,qj",
        "mqimqsdicmqsx<sqidUOIUZKJSHQS",
        "toto123", ""
    };
    WBoAuthCred known2 = (WBoAuthCred) {
        "example.org", "10921029",
        "qlmksd,qcpsdqslkdqs,kdqjsmdl,qj",
        "mqimqsdicmqsx<sqidUOIUZKJSHQS",
        "toto123", ""
    };
    bool  result1 = _TestConfigCompare(&known, &known2);
    bool  result2 = _TestConfigCompare(&known2, &known);
    ASSERT_EQ(true, result1);
    ASSERT_EQ(true, result2);
    PASS();
}

TEST wbac_compare_nok_1() {
    WBoAuthCred known = (WBoAuthCred) {
        "exAmple.org", "10921029",
        "qlmksd,qcpsdqslkdqs,kdqjsmdl,qj",
        "mqimqsdicmqsx<sqidUOIUZKJSHQS",
        "toto123", ""
    };
    WBoAuthCred known2 = (WBoAuthCred) {
        "example.org", "10921029",
        "qlmksd,qcpsdqslkdqs,kdqjsmdl,qj",
        "mqimqsdicmqsx<sqidUOIUZKJSHQS",
        "toto123", ""
    };
    bool  result1 = _TestConfigCompare(&known, &known2);
    bool  result2 = _TestConfigCompare(&known2, &known);
    ASSERT_EQ(false, result1);
    ASSERT_EQ(false, result2);
    PASS();
}

TEST wbac_compare_nok_2() {
    WBoAuthCred known = (WBoAuthCred) {
        "example.org", "10921029",
        "qlmksd,qcpsdqslkdqs,kdqjsmdl,qj",
        "mqimqsdicmqsx<sqidUOIUZKJSHQS",
        "toto123", ""
    };
    WBoAuthCred known2 = (WBoAuthCred) {
        "example.org", "10921829",
        "qlmksd,qcpsdqslkdqs,kdqjsmdl,qj",
        "mqimqsdicmqsx<sqidUOIUZKJSHQS",
        "toto123", ""
    };
    bool  result1 = _TestConfigCompare(&known, &known2);
    bool  result2 = _TestConfigCompare(&known2, &known);
    ASSERT_EQ(false, result1);
    ASSERT_EQ(false, result2);
    PASS();
}

TEST wbac_compare_nok_3() {
    WBoAuthCred known = (WBoAuthCred) {
        "example.org", "10921029",
        "qlmksd,qcpsdqslkdqs,kdqjsmdl,qj",
        "mqimqsdicmqsx<sqidUOIUZKJSHQS",
        "toto123", ""
    };
    WBoAuthCred known2 = (WBoAuthCred) {
        "example.org", "10921029",
        "qlmksd,qcpskdqs,kdqjsmdl,qj",
        "mqimqsdicmqsx<UZKJSHQS",
        "toto123", ""
    };
    bool  result1 = _TestConfigCompare(&known, &known2);
    bool  result2 = _TestConfigCompare(&known2, &known);
    ASSERT_EQ(false, result1);
    ASSERT_EQ(false, result2);
    PASS();
}

TEST wbac_compare_nok_4() {
    WBoAuthCred known = (WBoAuthCred) {
        "example.org", "10921029",
        "qlmksd,qcpsdqslkdqs,kdqjsmdl,qj",
        "mqimqsdicmqsx<sqidUOIUZKJSHQS",
        "toto123", ""
    };
    WBoAuthCred known2 = (WBoAuthCred) {
        "example.org", "10921029",
        "qlmksd,qcpsdqslkdqs,kdqjsmdl,qj",
        "mqimqsdicmqsx<sqidUOIUZKJSHQS",
        "toto7123", ""
    };
    bool  result1 = _TestConfigCompare(&known, &known2);
    bool  result2 = _TestConfigCompare(&known2, &known);
    ASSERT_EQ(false, result1);
    ASSERT_EQ(false, result2);
    PASS();
}

TEST wbac_compare_nok_5() {
    WBoAuthCred known = (WBoAuthCred) {
        "example.org", "10921029",
        "qlmksd,qcpsdqslkdqs,kdqjsmdl,qj",
        "mqimqsdicmqsx<sqidUOIUZKJSHQS",
        "toto123", ""
    };
    WBoAuthCred known2 = (WBoAuthCred) {
        "example.org", "10921029",
        "qlmksd,qcpsdqslkdqs,kdqjsmdl,qj",
        "mqimqsdicmqsx<sqidUOIUZKJSHQS",
        "toto123", "35714687687687"
    };
    bool  result1 = _TestConfigCompare(&known, &known2);
    bool  result2 = _TestConfigCompare(&known2, &known);
    ASSERT_EQ(false, result1);
    ASSERT_EQ(false, result2);
    PASS();
}

TEST wbac_compare_nok_6() {
    WBoAuthCred known = (WBoAuthCred) {
        "example.org", "10921029",
        "qlmksd,qcpsdqslkdqs,kdqjsmdl,qj",
        "mqimqsdicmqsx<sqidUOIUZKJSHQS",
        "toto123", ""
    };
    WBoAuthCred wac_default;
    WBConfigInit(&wac_default);
    bool  result1 = _TestConfigCompare(&known, &wac_default);
    bool  result2 = _TestConfigCompare(&wac_default, &known);
    WBConfigCleanup(&wac_default);
    ASSERT_EQ(false, result1);
    ASSERT_EQ(false, result2);
    PASS();
}

TEST wbac_compare_two_default() {
    WBoAuthCred wac_default;
    WBoAuthCred known;
    WBConfigInit(&wac_default);
    WBConfigInit(&known);
    bool  result1 = _TestConfigCompare(&known, &wac_default);
    bool  result2 = _TestConfigCompare(&wac_default, &known);
    WBConfigCleanup(&wac_default);
    WBConfigCleanup(&known);
    ASSERT_EQ(true, result1);
    ASSERT_EQ(true, result2);
    PASS();
}


SUITE(_wbac_compare) {
    RUN_TEST(wbac_compare_ok);
    RUN_TEST(wbac_compare_nok_1);
    RUN_TEST(wbac_compare_nok_2);
    RUN_TEST(wbac_compare_nok_3);
    RUN_TEST(wbac_compare_nok_4);
    RUN_TEST(wbac_compare_nok_5);
    RUN_TEST(wbac_compare_nok_6);
    RUN_TEST(wbac_compare_two_default);
}

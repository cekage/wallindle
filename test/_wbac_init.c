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

TEST wbac_init_ok() {
    WBoAuthCred wac_default;
    int result = WBConfigInit(&wac_default);
    WBConfigCleanup(&wac_default);
    ASSERT_EQ(WNDL_OK, result);
    PASS();
}

// Makes valgrind crying :/
TEST wbac_init_double_init() {
    WBoAuthCred wac_default;
//   WBConfigInit(&wac_default);
    int result = WBConfigInit(&wac_default);
    WBConfigCleanup(&wac_default);
    ASSERT_EQ(WNDL_OK, result);
    PASS();
}

TEST wbac_init_double_cleanup() {
    WBoAuthCred wac_default;
    int result = WBConfigInit(&wac_default);
    WBConfigCleanup(&wac_default);
    WBConfigCleanup(&wac_default);
    ASSERT_EQ(WNDL_OK, result);
    PASS();
}

TEST wbac_loop_init_cleanup() {
    WBoAuthCred wac_default;

    for (int i = 0; i < 8192; ++i) {
        int result = WBConfigInit(&wac_default);
        WBConfigCleanup(&wac_default);
        ASSERT_EQ(WNDL_OK, result);
    }

    PASS();
}

SUITE(_wbac_init) {
    RUN_TEST(wbac_init_ok);
    RUN_TEST(wbac_init_double_init);
    RUN_TEST(wbac_init_double_cleanup);
    RUN_TEST(wbac_loop_init_cleanup);
}

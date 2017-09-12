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

TEST conffile_check_size_min() {
    off_t size49 = GetFileSize("./files/size49.cfg", true);
    off_t size50 = GetFileSize("./files/size50.cfg", true);
    off_t size51 = GetFileSize("./files/size51.cfg", true);
    ASSERT_EQ(size49, -1);
    ASSERT_EQ(size50, 50);
    ASSERT_EQ(size51, 51);
    PASS();
}


TEST conffile_check_size_max() {
    off_t size4095 = GetFileSize("./files/size4095.cfg", true);
    off_t size4096 = GetFileSize("./files/size4096.cfg", true);
    off_t size4097 = GetFileSize("./files/size4097.cfg", true);
    off_t size4097bis = GetFileSize("./files/size4097.cfg", false);
    ASSERT_EQ(size4095, 4095);
    ASSERT_EQ(size4096, 4096);
    ASSERT_EQ(size4097, -1);
    ASSERT_EQ(size4097bis, 4097);
    PASS();
}


TEST conffile_check_config_get_ok() {
    WBoAuthCred known = (WBoAuthCred) {
        "https://wallabag.example.com",
        "2_1xyggA5982e8oscs08os4ckckw00gcscs4g404sg44gg4gowoo",
        "4u50vzwnrdwgo84c8wg8sgwskks888wskkc8o04o44kwg4080g",
        "root", "sV5G/aTjYRcNkSlTOsZuB78YG.", ""
    };
    WBoAuthCred a_wbc;
    WBConfigInit(&a_wbc);
    wd_result retourWD = _WBConfigGet(&a_wbc, "./files/wallindle.cfg");
    bool  result1 = _TestConfigCompare(&known, &a_wbc);
    bool  result2 = _TestConfigCompare(&a_wbc, &known);
    WBConfigCleanup(&a_wbc);
    ASSERT_EQ(retourWD, WNDL_OK);
    ASSERT_EQ(true, result1);
    ASSERT_EQ(true, result2);
    PASS();
}

TEST conffile_check_config_get_not_ok() {
    WBoAuthCred a_wbc;
    WBConfigInit(&a_wbc);
    wd_result retour50 = _WBConfigGet(&a_wbc, "./files/size50.cfg");
    wd_result retour51 = _WBConfigGet(&a_wbc, "./files/size51.cfg");
    WBConfigCleanup(&a_wbc);
    ASSERT_EQ(retour50, WNDL_ERROR);
    ASSERT_EQ(retour51, WNDL_ERROR);
    PASS();
}

TEST conffile_check_forgedurl_ok() {
    WBoAuthCred known = (WBoAuthCred) {
        "https://wallabag.example.com",
        "2_1xyggA5982e8oscs08os4ckckw00gcscs4g404sg44gg4gowoo",
        "4u50vzwnrdwgo84c8wg8sgwskks888wskkc8o04o44kwg4080g",
        "root", "sV5G/aTjYRcNkSlTOsZuB78YG.", ""
    };
    char* forged_url = WBConfigForgeoAuthURL(&known);
    const  char* WANTED =
        "https://wallabag.example.com/oauth/v2/token?grant_type=password&client_id=2_1xyggA5982e8oscs08os4ckckw00gcscs4g404sg44gg4gowoo&client_secret=4u50vzwnrdwgo84c8wg8sgwskks888wskkc8o04o44kwg4080g&username=root&password=sV5G/aTjYRcNkSlTOsZuB78YG.";
    ASSERT_STR_EQ(WANTED, forged_url);
    free(forged_url);
    PASS();
}

TEST conffile_check_forgedurl_not_ok() {
    WBoAuthCred known = (WBoAuthCred) {
        "https://wallabag.example.com",
        "2_1xyggA5982e8oscs08os4ckckw00gcscs4g404sg44gg4gowoo",
        "4u50vzwnrdwgo84c8wg8sgwskks888wskkc8o04o44kwg4080g",
        "root", "sV5G/aTjYRcNkSlTOsZuB78YG.", ""
    };
    char* forged_url = WBConfigForgeoAuthURL(&known);
    const  char* WANTED =
        "hXXps://wallabag.example.com/oauth/v2/token?grant_type=password&client_id=2_1xyggA5982e8oscs08os4ckckw00gcscs4g404sg44gg4gowoo&client_secret=4u50vzwnrdwgo84c8wg8sgwskks888wskkc8o04o44kwg4080g&username=root&password=sV5G/aTjYRcNkSlTOsZuB78YG.";
    const  bool  result = (0 != strcmp(WANTED, forged_url));
    free(forged_url);
    ASSERT(result);
    PASS();
}

TEST conffile_check_forgedurl_ok2() {
    WBoAuthCred a_wbc;
    WBConfigInit(&a_wbc);
    wd_result retourWD = _WBConfigGet(&a_wbc, "./files/wallindle.cfg");
    ASSERT_EQ(retourWD, WNDL_OK);
    char* forged_url = WBConfigForgeoAuthURL(&a_wbc);
    const  char* WANTED =
        "https://wallabag.example.com/oauth/v2/token?grant_type=password&client_id=2_1xyggA5982e8oscs08os4ckckw00gcscs4g404sg44gg4gowoo&client_secret=4u50vzwnrdwgo84c8wg8sgwskks888wskkc8o04o44kwg4080g&username=root&password=sV5G/aTjYRcNkSlTOsZuB78YG.";
    ASSERT_STR_EQ(WANTED, forged_url);
    WBConfigCleanup(&a_wbc);
    free(forged_url);
    PASS();
}
TEST conffile_check_forgedurl_not_ok2() {
    WBoAuthCred a_wbc;
    WBConfigInit(&a_wbc);
    wd_result retourWD = _WBConfigGet(&a_wbc, "./files/wallindle.cfg");
    ASSERT_EQ(retourWD, WNDL_OK);
    char* forged_url = WBConfigForgeoAuthURL(&a_wbc);
    const  char* WANTED =
        "hXXp://wallabag.example.com/oauth/v2/token?grant_type=password&client_id=2_1xyggA5982e8oscs08os4ckckw00gcscs4g404sg44gg4gowoo&client_secret=4u50vzwnrdwgo84c8wg8sgwskks888wskkc8o04o44kwg4080g&username=root&password=sV5G/aTjYRcNkSlTOsZuB78YG.";
    const  bool  result = (0 != strcmp(WANTED, forged_url));
    WBConfigCleanup(&a_wbc);
    free(forged_url);
    ASSERT(result);
    PASS();
}

TEST conffile_check_default() {
    WBoAuthCred a_wbc1;
    WBoAuthCred a_wbc2;
    WBConfigInit(&a_wbc1);
    WBConfigInit(&a_wbc2);
    wd_result retourWD1 = _WBConfigGet(&a_wbc1, "./files/wallindle.cfg");
    wd_result retourWD2 = WBConfigGet(&a_wbc2);
    bool  result1 = _TestConfigCompare(&a_wbc1, &a_wbc2);
    bool  result2 = _TestConfigCompare(&a_wbc2, &a_wbc1);
    WBConfigCleanup(&a_wbc1);
    WBConfigCleanup(&a_wbc2);
    ASSERT_EQ(retourWD1, WNDL_OK);
    ASSERT_EQ(retourWD2, WNDL_OK);
    ASSERT_EQ(true, result1);
    ASSERT_EQ(true, result2);
    PASS();
}

TEST conffile_check_nonexistant_config() {
    WBoAuthCred known = (WBoAuthCred) {
        "wallabag.example.com",
        "2_1xyggA5982e8oscs08os4ckckw00gcscs4g404sg44gg4gowoo",
        "4u50vzwnrdwgo84c8wg8sgwskks888wskkc8o04o44kwg4080g",
        "root", "sV5G/aTjYRcNkSlTOsZuB78YG.", ""
    };
    WBoAuthCred a_wbc;
    WBConfigInit(&a_wbc);
    int retourWD = _WBConfigGet(&a_wbc, "./files/nonexistant.cfg");
    bool  result1 = _TestConfigCompare(&known, &a_wbc);
    bool  result2 = _TestConfigCompare(&a_wbc, &known);
    WBConfigCleanup(&a_wbc);
    ASSERT_EQ(retourWD, WNDL_ERROR);
    ASSERT_EQ(false, result1);
    ASSERT_EQ(false, result2);
    PASS();
}

SUITE(_conffile) {
    RUN_TEST(conffile_check_size_min);
    RUN_TEST(conffile_check_size_max);
    RUN_TEST(conffile_check_config_get_ok);
    RUN_TEST(conffile_check_config_get_not_ok);
    RUN_TEST(conffile_check_forgedurl_ok);
    RUN_TEST(conffile_check_forgedurl_not_ok);
    RUN_TEST(conffile_check_forgedurl_ok2);
    RUN_TEST(conffile_check_forgedurl_not_ok2);
    RUN_TEST(conffile_check_default);
    RUN_TEST(conffile_check_nonexistant_config);
}

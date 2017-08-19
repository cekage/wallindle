# What is Wallindle
Wallindle is a native wallabag client for e-ink kindle. Wallindle fetch new articles in your Wallabag and add them to your kindle's unread ebooks.

The standard way to "read-later" an article on the kindle is using a "read-later" online service and transmit the saved article to the kindle using Amazon "by-email" transmission. It works very well and with a very easy minimal setup !

However it relies on a two services which can be shut down at any moment. Wallabag can be your very own "read-later" service and in this case, Wallindle allows your e-ink kindle fetching saved articles autonomously !

# What do I need ?
- An e-ink Kindle, [jailbroken](https://www.mobileread.com/forums/showthread.php?t=275881). If you don't undestand instructions, please do not attempt a jailbreak.
- A Wallabag account (either your own instance or [a official one](https://www.wallabag.it/) with some recent articles !

# Running Wallindle
## Building binary
- You need a "arm-linux-gnueabi-gcc" compiler. You can easily find one on internet.

If you are a gentoo user I strongly recommend you builduing your own with crossdev : 
```sh
$ crossdev --stable -v -t arm-linux-gnueabi
```
- Get Wallindle sources 
```
git clone https://github.com/cekage/wallindle.git
```
- Adjust binaries path and name in Makefile 
- Make arm binary
```sh
$ make arm
arm-linux-gnueabi-gcc -O0 -std=iso9899:1999   -mfpu=vfp -mfloat-abi=softfp -mtune=cortex-a8 -march=armv7-a wallindle.c  configmanager.c entries_common.c entries_parse.c http_request.c json_common.c json_entries_parse.c json_oauth_parse.c oauth_manager.c perform_entries.c shared.c lib/jsmn/jsmn.c -o wallindle lib/curl_arm_32/lib/libcurl.a -lrt -DCURL_STATICLIB -Ilib/curl_arm_32/include -lm

$ ls -alh wallindle
-rwxr-xr-x 1 k users 193K 19 août  16:03 wallindle

$ file wallindle
wallindle: ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux.so.3, for GNU/Linux 2.6.32, not stripped, with debug_info

```
- A oAuth2 credentials [follow instructions here](https://doc.wallabag.org/en/developer/api/oauth.html) 


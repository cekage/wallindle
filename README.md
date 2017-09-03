# What is Wallindle
Wallindle is a native wallabag client for e-ink kindle.
Wallindle fetch new articles in your Wallabag and add them to your
kindle's unread ebooks.

The standard way to "read-later" an article on the kindle is using a
"read-later" online service and transmit the saved article to the kindle using
Amazon "by-email" transmission.
It works very well and with a very easy minimal setup !

However it relies on a two services which can be shut down at any moment.
Wallabag can be your very own "read-later" service and in this case, Wallindle
allows your e-ink kindle fetching saved articles autonomously !

# What do I need ?
- An e-ink Kindle,
[jailbroken](https://www.mobileread.com/forums/showthread.php?t=275881).
If you don't undestand instructions, please do not attempt a jailbreak.

- A Wallabag account (either your own instance or
[a official one](https://www.wallabag.it/)) with some recent articles !

# Remerciements :

- Kindle jailbreak scene
- Gentoo Project for kindle toolchain
- Wallabag team
- Jsmn library for parsing jason in C
- libcurl team
- [Hervé](https://twitter.com/hervethouzard) for Wallabag support

# Running Wallindle
## Building binary

- You need a "arm-linux-gnueabi" compiler. You can easily find one in your
linux distro repository.

If you are a "Gentoo Linux" user I strongly recommend you builduing your own
toolchain with crossdev : 
```sh
$ crossdev --stable -v -t arm-linux-gnueabi
```
- Get Wallindle sources :
```
git clone https://github.com/cekage/wallindle.git
```
- Download libcurl, libssl, libcrypt and libcrypto from the kindle using download_kindle_lib.sh :
```
$ cat download_kindle_lib.sh 
#!/usr/bin/env bash

DEST=lib/curlkindle/lib
SRC=/usr/lib
KINDLEIP=192.168.15.244

for lib in ssl curl crypt
do
	scp root@${KINDLEIP}:${SRC}/lib${lib}*so ${DEST}
done;

$ ./download_kindle_lib.sh 


Welcome to Kindle!

root@192.168.15.244's password: 
libssl.so              100%  287KB   6.0MB/s 286.7KB/s   00:00 


Welcome to Kindle!

root@192.168.15.244's password: 
libcurl.so             100%  208KB   5.9MB/s 207.9KB/s   00:00 


Welcome to Kindle!

root@192.168.15.244's password: 
libcrypt.so            100%   38KB   4.6MB/s  37.5KB/s   00:00 
libcrypto.so           100% 1393KB   6.2MB/s   1.4MB/s   00:00

$ ls -alh lib/curlkindle/lib/*.so
-rwxr-xr-x 1 k users 1,4M  3 sept. 20:53 lib/curlkindle/lib/libcrypto.so
-rw-r--r-- 1 k users  38K  3 sept. 20:53 lib/curlkindle/lib/libcrypt.so
-rwxr-xr-x 1 k users 208K  3 sept. 20:53 lib/curlkindle/lib/libcurl.so
-rwxr-xr-x 1 k users 287K  3 sept. 20:53 lib/curlkindle/lib/libssl.so

```
- Adjust binaries path and name in Makefile 
- Make kindle native binary :
```sh
$ make arm
arm-linux-gnueabi-gcc -O0 -std=iso9899:1999   -mfpu=vfp -mfloat-abi=softfp -mtune=cortex-a8 -march=armv7-a wallindle.c  configmanager.c entries_common.c entries_parse.c http_request.c json_common.c json_entries_parse.c json_oauth_parse.c oauth_manager.c perform_entries.c shared.c lib/jsmn/jsmn.c -o wallindle lib/curl_arm_32/lib/libcurl.a -lrt -DCURL_STATICLIB -Ilib/curl_arm_32/include -lm

$ ls -alh wallindle
-rwxr-xr-x 1 k users 193K 19 août  16:03 wallindle

$ file wallindle
wallindle: ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux.so.3, for GNU/Linux 2.6.32, not stripped, with debug_info
```
## Gathering informations
- Get your very own oAuth2 credentials [follow instructions here](https://doc.wallabag.org/en/developer/api/oauth.html)

- Prepare configuration :
Basically, concatening (as a single line) host, client\_id, client\_secret,
username and password in a file.
```sh
# echo "https://wallabag.example.com 2_1xyggA5982e8oscs08os4ckckw00gcscs4g404sg44gg4gowoo 4u50vzwnrdwgo84c8wg8sgwskks888wskkc8o04o44kwg4080g root sV5G/aTjYRcNkSlTOsZuB78YG." > ./wallindle.cfg
```

## Uploading to kindle

Do as you want : by command line, file manager or even ssh it doesn't matter. 
Storing both wallindle binary and configuration in the same folders as
epub is a good option. Assuming that, your binary and config file are now
stored in /mnt/base-us/documents/

## Testing on kindle
```sh
$ ssh root@192.168.15.244


Welcome to Kindle!

root@192.168.15.244's password: 
#################################################
#  N O T I C E  *  N O T I C E  *  N O T I C E  # 
#################################################
Rootfs is mounted read-only. Invoke mntroot rw to
switch back to a writable rootfs.
#################################################

[root@kindle root]# cp /mnt/base-us/documents/wallindle* /tmp

[root@kindle root]# cd /tmp 

[root@kindle tmp]# ls -alh wallindle*
-rwxr-xr-x    1 root     root       192.8k Aug 19 14:34 wallindle
-rwxr-xr-x    1 root     root          168 Aug 19 14:34 wallindle.cfg

[root@kindle tmp]# ./wallindle
 token_count = 11
(...)
created_at= 2017-08-19T

MischiefManaged!
```
If you see "MischiefManaged!" then everything goes well :-). Otherwise
you will see an error you can retrieve inside code source or by pinging me
on [twitter](https://twitter.com/cekagedev)


In my case 7 files were downloaded :

```sh
[root@kindle tmp]# ls -alh /mnt/base-us/documents/wallabag/*mobi
-rwxr-xr-x    1 root     root       113.4k Aug 19 14:35 /mnt/base-us/documents/wallabag/0000010f.mobi
-rwxr-xr-x    1 root     root         4.5k Aug 19 14:35 /mnt/base-us/documents/wallabag/00000111.mobi
-rwxr-xr-x    1 root     root         1.5k Aug 19 14:35 /mnt/base-us/documents/wallabag/00000112.mobi
-rwxr-xr-x    1 root     root         3.6k Aug 19 14:35 /mnt/base-us/documents/wallabag/00000125.mobi
-rwxr-xr-x    1 root     root         3.3k Aug 19 14:35 /mnt/base-us/documents/wallabag/00000126.mobi
-rwxr-xr-x    1 root     root         9.4k Aug 19 14:35 /mnt/base-us/documents/wallabag/00000127.mobi
-rwxr-xr-x    1 root     root        16.0k Aug 19 14:35 /mnt/base-us/documents/wallabag/00000128.mobi
```

# Let's the kindle fetching by itself
Now this is time for bad to really bad advices. Letting a non Amazon approved 
binary running by itself is a really bad idea. But :

- Even if kindle has other user accounts, only root can write on Document
folders. In every case, wallindle must runs with root privilege, and it's a
very bad news.

- None of the file stored on Document can be executed. No known workaround with
the kernel 2.6.31 running in the Kindle.

- The Document directory is a fat32 filesystem, there is no rights management
with it. Every files can be rw by owner and r by group and others.

## Bad idea #1 : running as root a non protected binary

*Keep in mind, it's a really bad idea : this create a very serious security
issue*

1. Storing wallindle and wallindle.cfg on Documents
2. Copy both from Documents to /tmp/
3. Launch wallindle ( /mnt/base-us/documents/wallabag directory in hardcoded )
4. crontab that

```sh
$ ssh root@192.168.15.244


Welcome to Kindle!

root@192.168.15.244's password: 
#################################################
#  N O T I C E  *  N O T I C E  *  N O T I C E  # 
#################################################
Rootfs is mounted read-only. Invoke mntroot rw to
switch back to a writable rootfs.
#################################################

[root@kindle root]# mntroot rw
system: I mntroot:def:Making root filesystem writeable
/dev/mmcblk0p1 on / type ext3 (rw,noatime,nodiratime)


[root@kindle root]# nano /etc/crontab/root 
```
Add this line :

```
00  */4 * * * (cd /mnt/us/documents/ && (./wallindle > wallindle.log))
```

Verify the new content
```sh
[root@kindle root]# cat /etc/crontab/root 
*/15 * * * * /usr/sbin/checkpmond
*/15 * * * * /usr/sbin/tinyrot
*/60 * * * * /usr/sbin/loginfo tmpfs
*/60 * * * * /usr/sbin/loginfo localVars
*/60 * * * * /usr/sbin/loginfo memusedump
*/15 * * * * /usr/sbin/loginfo powerdcheck

00  */4 * * * (cd /mnt/us/documents/ && (./wallindle > wallindle.log))
```

Remount / read only :
```sh
[root@kindle root]# mntroot ro
system: I mntroot:def:Making root filesystem read-only
/dev/mmcblk0p1 on / type ext3 (ro,noatime,nodiratime)

```

Restart cron agent :
```sh
[root@kindle root]# pkill crond && /usr/sbin/crond -c /etc/crontab/
```

Remember kids, this file will be run as root. Do you *really* want to transform
your kindle in a piece of garbage ? I'm sure not.

## Bad idea #2 : running as root a protected binary

Same as the first bad idea except you don't store wallindle in
/mnt/base-us/documents/ but elsewhere on the rom of the kindle. In this case
nobody can alter the binary but do you really want to alter the rom file
structure list ? 

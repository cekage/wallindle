CC=gcc
CFLAGS=-O0 -g -m64 -std=iso9899:1999 -pedantic -Wall -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes \
	-Wno-missing-braces -Wextra -Wno-missing-field-initializers -Wformat=2 \
    -Wswitch-default -Wswitch-enum -Wcast-align -Wpointer-arith \
    -Wbad-function-cast -Wstrict-overflow=5 -Wstrict-prototypes -Winline \
    -Wundef -Wnested-externs -Wcast-qual -Wshadow -Wunreachable-code \
    -Wlogical-op -Wfloat-equal -Wstrict-aliasing=2 -Wredundant-decls \
    -Wold-style-definition
    #-Werror

ARM_CFLAGS=-mfpu=vfp -mfloat-abi=softfp -mtune=cortex-a8 -march=armv7-a

LDFLAGS=lib/curl_x86_64/lib/libcurl.a -DCURL_STATICLIB -Ilib/curl_x86_64/include -lm
LDFLAGS=`/usr/bin/curl-config --libs` -lm

EXEC=wallindle

OTHERC=configmanager.c entries_common.c http_request.c \
	   json_common.c json_entries_parse.c json_oauth_parse.c \
	   perform_entries.c shared.c \
	   lib/jsmn/jsmn.c

all: clean build exec

build:
	$(CC) $(CFLAGS) $(EXEC).c $(OTHERC) -o $(EXEC) $(LDFLAGS)

exec:
	./$(EXEC)

clean:
	rm -rf $(EXEC)

clear: clean

mrproper: clean
	rm -rf $(EXEC)

astyle:
	astyle --mode=c --style=attach --align-pointer=type --align-reference=name --add-braces \
	--add-one-line-braces --attach-classes --attach-closing-while --max-code-length=80 \
	--indent=spaces --break-blocks --suffix=none --lineend=linux \
	--pad-header --pad-oper --pad-comma --indent=spaces=4 --indent-col1-comments \
	 *.c *.h

arm:
	test -s lib/curlkindle/lib/libcrypto.so || { echo "libcurl.so not found. Run download_kindle_lib.sh (see README.md)."; exit 1; }
#	$(eval CC := arm-linux-gnueabi-gcc)
	$(eval CC := arm-kindle-linux-gnueabi-gcc)
	$(eval CFLAGS := -O0 -std=iso9899:1999  )
	$(eval LDFLAGS :=  `./lib/curlkindle/bin/curl-config --libs` -lm)
	$(CC) $(CFLAGS) $(ARM_CFLAGS) wallindle.c  $(OTHERC) -o $(EXEC) $(LDFLAGS)

codewarnings:
#	clang-tidy wallindle.c -header-filter=.* -checks=*,clang-analyzer-*,-clang-analyzer-cplusplus* -- -std=iso9899:1999
#	clang-tidy wallindle.c $(OTHERC) -header-filter=.* -checks=*,-clang-analyzer-alpha.unix.Stream,-clang-analyzer-cplusplus*,llvm-header-guard,llvm-include-order --  -std=iso9899:1999 -lm
	clang-tidy wallindle.c $(OTHERC) -header-filter=.* -checks=*,-*,llvm-include-order --  -std=iso9899:1999 -lm

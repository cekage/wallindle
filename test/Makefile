CC=gcc
CFLAGS=-O0 -g -m64 -std=iso9899:1999
# -Wunused-function
#    -pedantic -Wall -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes \
#	-Wno-missing-braces -Wextra -Wno-missing-field-initializers -Wformat=2 \
#    -Wswitch-default -Wswitch-enum -Wcast-align -Wpointer-arith \
#    -Wbad-function-cast -Wstrict-overflow=5 -Wstrict-prototypes -Winline \
#    -Wundef -Wnested-externs -Wcast-qual -Wshadow -Wunreachable-code \
#    -Wlogical-op -Wfloat-equal -Wstrict-aliasing=2 -Wredundant-decls \
#    -Wold-style-definition
#    -Werror

LDFLAGS=../lib/curl_x86_64/lib/libcurl.a -DCURL_STATICLIB -I../lib/curl_x86_64/include -lm

EXEC=alltests

OTHERC=../shared.c ../lib/jsmn/jsmn.c

#	../configmanager.c ../entries_common.c ../entries_parse.c ../http_request.c \
#	   ../json_common.c ../json_entries_parse.c ../json_oauth_parse.c ../oauth_manager.c \
#	   ../perform_entries.c ../shared.c \
#	   ../lib/jsmn/jsmn.c


all: clean build exec

build:
	$(CC) $(CFLAGS) $(EXEC).c $(OTHERC) -o $(EXEC) $(LDFLAGS)

exec:
	./$(EXEC)

valgrind: clean build
	valgrind --leak-check=full ./$(EXEC)

clean:
	rm -rf $(EXEC)

clear: clean

mrproper: clean
	rm -rf $(EXEC)



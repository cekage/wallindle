#!/usr/bin/env bash

DEST=lib/curlkindle/lib
SRC=/usr/lib
KINDLEIP=192.168.15.244

for lib in ssl curl crypt
do
	scp root@${KINDLEIP}:${SRC}/lib${lib}*so ${DEST}
done;


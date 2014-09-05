#!/bin/bash

STARTTIME=$(date +%s)
PROJECTDIR=`pwd`

g++ -fPIC -shared -g -o $PROJECTDIR/bin/libwebsocket.so                   \
	$PROJECTDIR/include/base64/base64.cpp                                 \
	$PROJECTDIR/include/sha1/sha1.cpp                                     \
	$PROJECTDIR/*.cpp                                                     \
	                                                                      \
	-ldl

ENDTIME=$(date +%s)

printf "\n\nBuild Time: \033[32m$(($ENDTIME - $STARTTIME)) seconds\033[0m.\n\n"

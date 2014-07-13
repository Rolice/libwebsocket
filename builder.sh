#!/bin/bash

STARTTIME=$(date +%s)
PROJECTDIR=`pwd`

g++ -fPIC -shared -o $PROJECTDIR/bin/libwebsocket.so -ldl					\
	$PROJECTDIR/include/base64/base64.cpp									\
	$PROJECTDIR/include/sha1/sha1.cpp										\
	$PROJECTDIR/*.cpp

ENDTIME=$(date +%s)

printf "\n\nBuild Time: \033[32m$(($ENDTIME - $STARTTIME)) seconds\033[0m.\n\n"

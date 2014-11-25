#!/bin/sh
if [ ! -d ./bin ]; then
 mkdir bin
fi
cc -o bin/$1 src/powernotifyd.c  -framework IOKit -framework CoreFoundation

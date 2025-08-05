#!/bin/bash

OS_NAME=`uname -o 2>/dev/null || uname -s`

if [ $OS_NAME == "Msys" ] || [ $OS_NAME == "MS/Windows" ]; then
    GLFLAG="-lopengl32"
elif [ $OS_NAME == "Darwin" ]; then
    GLFLAG="-framework OpenGL"
else
    GLFLAG="-lGL"
fi

ARG="${1:-sdl2}"

if [ $ARG == "sdl3" ]; then
    CFLAGS="$CFLAGS -DSDL3_RENDERER"
    SDLFLAG="`pkg-config --libs sdl3`"
else
    SDLFLAG="`sdl2-config --libs`"
fi

CFLAGS="$CFLAGS -I../src -Wall -std=c11 -pedantic $SDLFLAG $GLFLAG -lm -O3 -g"

gcc main.c renderer.c ../src/microui.c $CFLAGS


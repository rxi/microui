#!/bin/sh

OS_NAME=`uname -o 2>/dev/null || uname -s`

case "$OS_NAME" in
Msys)
    GLFLAG="-lopengl32";;
Darwin)
    GLFLAG="-framework OpenGL";;
*)
    GLFLAG="-lGL";;
esac

CFLAGS="-I../src -Wall -std=c11 -pedantic `sdl2-config --libs` $GLFLAG -lm -O3 -g"

gcc main.c renderer.c ../src/microui.c $CFLAGS


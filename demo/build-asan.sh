#!/usr/bin/env bash

# NOTE: On Windows, this was tested using MSYS2 CLANG64.
#       The MSYS2 MinGW-w64 (GCC based) toolchain does not ship a usable ASan runtime.

set -e

UNAME_S="$(uname -s)"

# OpenGL link flag
case "$UNAME_S" in
  MSYS*|MINGW*|CYGWIN*)
    GLFLAG="-lopengl32"
    ;;
  Darwin)
    GLFLAG="-framework OpenGL"
    ;;
  *)
    GLFLAG="-lGL"
    ;;
esac

CFLAGS="-I../src -Wall -fsanitize=address,alignment,undefined -fno-omit-frame-pointer -O1 -std=c11 -pedantic -g $(sdl2-config --cflags)"
LDLIBS="$(sdl2-config --libs) $GLFLAG -lm"

cc $CFLAGS -o asan main.c renderer.c ../src/microui.c $LDLIBS

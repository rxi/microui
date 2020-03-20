#!/bin/bash
if test `uname -o` == "Msys"; then
    GL="opengl32"
else
    GL="GL"
fi
gcc main.c renderer.c ../src/microui.c -I../src\
    -Wall -std=c11 -pedantic `sdl2-config --libs` -l${GL} -lm -O3 -g


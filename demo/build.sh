#!/bin/bash
gcc main.c renderer.c ../src/microui.c -I../src\
    -Wall -std=c11 -pedantic -lSDL2 -lGL -lm -O3 -g

#!/bin/bash

CFLAGS="-I../../src -Wall -std=c11 -pedantic -lgdi32 -Os -g"


gcc main.c renderer.c ../../src/microui.c $CFLAGS


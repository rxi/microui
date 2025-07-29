# Simple Makefile for ncurses-based microui-config

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -O2
LDFLAGS = -lncurses

SRC     = Makefiles/microui-config.c
BIN     = microui-config

.PHONY: all clean

all: 
	$(CC) $(CFLAGS) $(SRC) -o $(BIN) $(LDFLAGS)
    
clean: 
	rm -f $(BIN)
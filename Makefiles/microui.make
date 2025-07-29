CC      = gcc
PREFIX = /usr/local/microui
INCLUDE_DIR = $(PREFIX)/include
BIN_DIR = $(PREFIX)/bin
MAKEFILES_DIR = $(PREFIX)/Makefiles

CFLAGS="-I$(INCLUDE_DIR) -Wall -std=c11 -pedantic `sdl2-config --libs` $GLFLAG -lm -O3 -g"

.PHONY: all clean

all: 
# prompt for main c file
	@read -p "Enter the main C file (default: src/main.c): " MAIN_C
# prompt for binary name
	@read -p "Enter the binary name (default: microui): " BIN
# use default values if empty
	MAIN_C=${MAIN_C:-src/main.c}
	BIN=${BIN:-microui}
# check if main C file exists
	@if [ ! -f $(MAIN_C) ]; then \
		echo "Error: Main C file $(MAIN_C) does not exist."; \
		exit 1; \
	fi
# compile job
	$(CC) $(CFLAGS) $(MAIN_C) -o $(BIN) $(LDFLAGS)

clean: 
	rm -f $(BIN)
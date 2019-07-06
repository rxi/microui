UNAME_S = $(shell uname -s)

CFLAGS += -Wall -pedantic -std=c11
LDFLAGS = -lSDL2 -lm

ifeq ("$(UNAME_S)", "Darwin")
	LDFLAGS += -framework OpenGL -framework Cocoa
else
	LDFLAGS += -lGL
endif

demo:
	$(CC) -o demo main.c renderer.c ../src/microui.c -I../src $(CFLAGS) $(LDFLAGS)

.DEFAULT_GOAL := demo

.PHONY: clean demo
clean:
	rm -f demo


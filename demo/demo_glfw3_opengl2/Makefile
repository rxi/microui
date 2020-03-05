#CC = gcc

MICROUI_DICT = ../../src

EXE = demo_glfw3_opengl2
SOURCES = main.c ../microui_demo.c
SOURCES += ../atlas.c
SOURCES += ../microui_impl_glfw3.c ../microui_impl_opengl2.c
SOURCES += $(MICROUI_DICT)/microui.c
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))


## COMPILE FLAGS

CFLAGS = -I$(MICROUI_DICT) -I..
CFLAGS += -O3 -g -Wall -pedantic -std=c11
CFLAGS += `pkg-config --cflags glfw3`
LIBS = -lGL -lm `pkg-config --libs glfw3`


## BUILD RULES

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o:../%.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o:$(MICROUI_DICT)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Finish to build '"$^"'

$(EXE): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)


## HEAD FILE DEPENDENCIES

microui.o     : $(MICROUI_DICT)/microui.h
microui_demo.o: $(MICROUI_DICT)/microui.h ../microui_demo.h
atlas.o       : $(MICROUI_DICT)/microui.h ../atlas.h

microui_impl_opengl2.o: $(MICROUI_DICT)/microui.h ../microui_impl_opengl2.h ../atlas.h
microui_impl_glfw3.o  : $(MICROUI_DICT)/microui.h ../microui_impl_glfw3.h ../atlas.h
main.o                : $(MICROUI_DICT)/microui.h ../microui_demo.h ../microui_impl_glfw3.h ../microui_impl_opengl2.h

.PHONY: all clean
clean:
	rm -f $(EXE) $(OBJS)
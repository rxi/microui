include microui-config.make

CC := gcc
CFILES := src/microui.c \
	 demo/renderer.c \
	 demo/atlas.inl

CHEADERS := src/microui.h \
	demo/renderer.h

MAKEFILES = Makefiles/microui.make \
	Makefiles/microui-make.sh \
#	Makefiles/microui-config.c \ 
	Makefiles/microui.sh \
	Makefiles/microui.csh

INSTALL_PATH = /usr/local/microui

BINS := Makefiles/microui-config

install:
	echo "Make sure to run make with sudo!"
	@mkdir -p $(INSTALL_PATH)/{include,bin,Makefiles}
	@cp -r $(MAKEFILES) $(INSTALL_PATH)/Makefiles/
	@cp -r $(CFILES) $(INSTALL_PATH)/include
	@cp -r $(BINS) $(INSTALL_PATH)/bin

uninstall:
	@rm -rf $(INSTALL_PATH)

shellhelp:
	echo "Add /usr/local/microui/Makefiles/microui.sh to your path! (by using 'source' in your shell init file)" 
	echo "echo '. /usr/local/microui/Makefiles/microui.sh' >> ~/.bashrc"

.PHONY install uninstall shellhelp

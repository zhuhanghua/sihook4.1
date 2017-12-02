# A simple makefile generator by KiSoft, 2010. mailto: kisoft@rambler.ru
# version: 0.3.12.5

# Project Variables start
CPP=mingw32-g++.exe
CC=mingw32-gcc.exe
LD=mingw32-g++.exe
LIB=ar.exe
WINDRES=windres.exe
# Project Variables end

# Target: Release

# define res
TAB_CTL_SUB_MENU_NAME = tabsubmenu
TAB_CTL_SUB_MENU_RES = $(TAB_CTL_SUB_MENU_NAME).res

#define objs
OBJS_RELEASE=obj\Release\siframe.o obj\Release\sihook.o obj\Release\sisw.o obj\Release\tabctl.o obj\Release\utils.o obj\Release\mdiclient.o obj\Release\tabsubmenu.o $(TAB_CTL_SUB_MENU_RES)
PROJECT_FILE_PATH=D:\Tools\SiHook
Release: bin/Release/sihook.dll

bin/Release/sihook.dll: $(OBJS_RELEASE)
	@echo Building dynamic library bin\Release\sihook.dll
	@mingw32-g++.exe -shared -Wl,--output-def=bin\Release\libsihook.dll.def -Wl,--out-implib=bin\Release\libsihook.dll.a -Wl,--dll  $(OBJS_RELEASE)  -o bin\Release\sihook.dll -s -luser32 -lgdi32 -lcomctl32 #-lCoreimm32

obj\Release\siframe.o: $(PROJECT_FILE_PATH)\siframe.c
	@echo Compiling: siframe.c
	@mingw32-gcc.exe  -O2 -Wall -DBUILD_DLL     -c siframe.c -o obj\Release\siframe.o

obj\Release\sihook.o: $(PROJECT_FILE_PATH)\sihook.c
	@echo Compiling: sihook.c
	@mingw32-gcc.exe  -O2 -Wall -DBUILD_DLL     -c sihook.c -o obj\Release\sihook.o

obj\Release\sisw.o: $(PROJECT_FILE_PATH)\sisw.c
	@echo Compiling: sisw.c
	@mingw32-gcc.exe  -O2 -Wall -DBUILD_DLL     -c sisw.c -o obj\Release\sisw.o

obj\Release\tabctl.o: $(PROJECT_FILE_PATH)\tabctl.c
	@echo Compiling: tabctl.c
	@mingw32-gcc.exe  -O2 -Wall -DBUILD_DLL     -c tabctl.c -o obj\Release\tabctl.o

obj\Release\utils.o: $(PROJECT_FILE_PATH)\utils.c
	@echo Compiling: utils.c
	@mingw32-gcc.exe  -O2 -Wall -DBUILD_DLL     -c utils.c -o obj\Release\utils.o

obj\Release\mdiclient.o: $(PROJECT_FILE_PATH)\mdiclient.c
	@echo Compiling: mdiclient.c
	@mingw32-gcc.exe  -O2 -Wall -DBUILD_DLL     -c mdiclient.c -o obj\Release\mdiclient.o

obj\Release\tabsubmenu.o: $(PROJECT_FILE_PATH)\tabsubmenu.c
    @echo Compiling: tabsubmenu.c
    @mingw32-gcc.exe  -O2 -Wall -DBUILD_DLL     -c mdiclient.c -o obj\Release\tabsubmenu.o

siframe.c: siframe.h tabctl.h const.h

sihook.c: siframe.h mdiclient.h

mdiclient.c: mdiclient.h sisw.h tabctl.h const.h

tabctl.c: const.h utils.h tabctl.h tabsubmenu.h

sisw.c: tabctl.h const.h utils.h sisw.h

utils.c: utils.h

tabsubmenu.c: tabsubmenu.h

.rc.res:
	rc $<

.PHONY: clean_Release

clean_Release:
	@echo Delete $(OBJS_RELEASE) bin\Release\sihook.dll
	-@del /f $(OBJS_RELEASE) bin\Release\sihook.dll

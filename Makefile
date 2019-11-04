# NOTE: to compile with MinGW32, use:
# make CC=i686-w64-mingw32-gcc CXX=i686-w64-mingw32-g++ WINDRES=i686-w64-mingw32-windres \
#   SDL_CONFIG=/opt/local/i686-w64-mingw32/bin/sdl2-config OUTPUT=SyobonAction.exe

PREFIX=./data

CFLAGS=-O2 -DPREFIX=\"$(PREFIX)/\"
LDFLAGS=

CC=gcc
CXX=g++
SDL_CONFIG=sdl2-config
WINDRES=
OUTPUT = SyobonAction

all: ${OUTPUT}

ifeq ($(WINDRES),)
RESOURCE_OBJ =
else
RESOURCE_OBJ = SyobonAction_rc.o
endif

clean:
	-rm -f main.o loadg.o DxLib.o sdlgfx/SDL_gfxPrimitives.o ${RESOURCE_OBJ} ${OUTPUT}

distclean: clean

${OUTPUT}: main.o loadg.o DxLib.o sdlgfx/SDL_gfxPrimitives.o ${RESOURCE_OBJ}
	${CXX} main.o loadg.o DxLib.o sdlgfx/SDL_gfxPrimitives.o ${RESOURCE_OBJ} \
		-o SyobonAction `${SDL_CONFIG} --libs` \
		-lSDL2_mixer $(LDFLAGS)

main.o:main.cpp
	${CXX} `${SDL_CONFIG} --cflags` $(CFLAGS) -I./sdlgfx -c main.cpp -o main.o

loadg.o:loadg.cpp
	${CXX} `${SDL_CONFIG} --cflags` $(CFLAGS) -I./sdlgfx -c loadg.cpp -o loadg.o

DxLib.o:DxLib.cpp
	${CXX} `${SDL_CONFIG} --cflags` $(CFLAGS) -I./sdlgfx -c DxLib.cpp -o DxLib.o

sdlgfx/SDL_gfxPrimitives.o:sdlgfx/SDL_gfxPrimitives.c
	${CC} `${SDL_CONFIG} --cflags` $(CFLAGS) -I./sdlgfx -c sdlgfx/SDL_gfxPrimitives.c -o sdlgfx/SDL_gfxPrimitives.o

SyobonAction_rc.o:SyobonAction.rc
	${WINDRES} -i SyobonAction.rc -o SyobonAction_rc.o

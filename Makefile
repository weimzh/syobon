PREFIX=./data

CFLAGS=-O2 -DPREFIX=\"$(PREFIX)/\"
LDFLAGS=

CC=gcc
CXX=g++

all: SyobonAction

clean:
	-rm -f main.o loadg.o DxLib.o sdlgfx/SDL_gfxPrimitives.o SyobonAction

distclean: clean

SyobonAction: main.o loadg.o DxLib.o sdlgfx/SDL_gfxPrimitives.o
	${CXX} main.o loadg.o DxLib.o sdlgfx/SDL_gfxPrimitives.o \
		-o SyobonAction `sdl-config --libs` \
		-lSDL_mixer $(LDFLAGS)

main.o:main.cpp
	${CXX} `sdl-config --cflags` $(CFLAGS) -I./sdlgfx -c main.cpp -o main.o

loadg.o:loadg.cpp
	${CXX} `sdl-config --cflags` $(CFLAGS) -I./sdlgfx -c loadg.cpp -o loadg.o

DxLib.o:DxLib.cpp
	${CXX} `sdl-config --cflags` $(CFLAGS) -I./sdlgfx -c DxLib.cpp -o DxLib.o

sdlgfx/SDL_gfxPrimitives.o:sdlgfx/SDL_gfxPrimitives.c
	${CC} `sdl-config --cflags` $(CFLAGS) -I./sdlgfx -c sdlgfx/SDL_gfxPrimitives.c -o sdlgfx/SDL_gfxPrimitives.o


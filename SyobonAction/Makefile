CFLAGS=-O2 -DPREFIX=\"./data/\"
LDFLAGS=

all: SyobonAction

clean:
	-rm -f main.o loadg.o DxLib.o sdlgfx/SDL_gfxPrimitives.o SyobonAction

distclean: clean

SyobonAction: main.o loadg.o DxLib.o sdlgfx/SDL_gfxPrimitives.o
	g++ main.o loadg.o DxLib.o sdlgfx/SDL_gfxPrimitives.o \
		-o SyobonAction `sdl-config --libs` \
		-lSDL_image -lSDL_mixer $(LDFLAGS)

main.o:main.cpp
	g++ `sdl-config --cflags` $(CFLAGS) -I./sdlgfx -c main.cpp -o main.o

loadg.o:loadg.cpp
	g++ `sdl-config --cflags` $(CFLAGS) -I./sdlgfx -c loadg.cpp -o loadg.o

DxLib.o:DxLib.cpp
	g++ `sdl-config --cflags` $(CFLAGS) -I./sdlgfx -c DxLib.cpp -o DxLib.o

sdlgfx/SDL_gfxPrimitives.o:sdlgfx/SDL_gfxPrimitives.c
	gcc `sdl-config --cflags` $(CFLAGS) -I./sdlgfx -c sdlgfx/SDL_gfxPrimitives.c -o sdlgfx/SDL_gfxPrimitives.o


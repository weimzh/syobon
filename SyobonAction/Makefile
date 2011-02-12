CFLAGS=-O2 -DPREFIX=\"./data/\"
LDFLAGS=

all: SyobonAction

clean:
	-rm -f main.o loadg.o DxLib.o SyobonAction

distclean: clean

SyobonAction: main.o loadg.o DxLib.o
	g++ main.o loadg.o DxLib.o -o SyobonAction `sdl-config --libs` \
		-lSDL_gfx -lSDL_image -lSDL_mixer -lSDL_ttf $(LDFLAGS)

main.o:main.cpp
	g++ `sdl-config --cflags` $(CFLAGS) -c main.cpp -o main.o

loadg.o:loadg.cpp
	g++ `sdl-config --cflags` $(CFLAGS) -c loadg.cpp -o loadg.o

DxLib.o:DxLib.cpp
	g++ `sdl-config --cflags` $(CFLAGS) -c DxLib.cpp -o DxLib.o

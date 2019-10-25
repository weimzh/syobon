PREFIX=./data

HOMEBREW_DIR=/usr/local

CFLAGS=-O2 -DPREFIX=\"$(PREFIX)/\" `sdl2-config --cflags`
LDFLAGS=${HOMEBREW_DIR}/lib/libSDL2.a ${HOMEBREW_DIR}/lib/libSDL2_mixer.a ${HOMEBREW_DIR}/lib/libSDL2main.a \
	-lm -liconv -Wl,-framework,CoreAudio -Wl,-framework,AudioToolbox -Wl,-framework,ForceFeedback -lobjc \
	-Wl,-framework,CoreVideo -Wl,-framework,Cocoa -Wl,-framework,Carbon -Wl,-framework,IOKit \
	-Wl,-weak_framework,QuartzCore -Wl,-weak_framework,Metal -L${HOMEBREW_DIR}/lib \
	-lmodplug ${HOMEBREW_DIR}/lib/libogg.a ${HOMEBREW_DIR}/lib/libvorbis.a ${HOMEBREW_DIR}/lib/libvorbisfile.a

CC=gcc
CXX=g++

all: SyobonAction.app/Contents/MacOS/SyobonAction

clean:
	-rm -rf SyobonAction.app ../main.o ../loadg.o ../DxLib.o ../sdlgfx/SDL_gfxPrimitives.o SyobonAction

SyobonAction.app/Contents/MacOS/SyobonAction: SyobonAction SyobonAction.icns
	-@mkdir -p SyobonAction.app/Contents/MacOS
	-@mkdir -p SyobonAction.app/Contents/Resources
	-@mkdir -p SyobonAction.app/Contents/Frameworks
	cp SyobonAction SyobonAction.app/Contents/MacOS/SyobonAction
	cp -r English.lproj SyobonAction.app/Contents/Resources/
	cp SyobonAction.icns SyobonAction.app/Contents/Resources/
	cp Info.plist SyobonAction.app/Contents/
	cp -r ../data SyobonAction.app/Contents/Resources/
	rm -f SyobonAction.app/Contents/Resources/data/res/overlay.bmp
	cp ${HOMEBREW_DIR}/lib/libmodplug.dylib SyobonAction.app/Contents/Frameworks
	install_name_tool -change `otool -L SyobonAction|grep libmodplug|awk '{print $$1;}'` @executable_path/../Frameworks/libmodplug.dylib \
		SyobonAction.app/Contents/MacOS/SyobonAction

distclean: clean

SyobonAction: ../main.o ../loadg.o ../DxLib.o ../sdlgfx/SDL_gfxPrimitives.o
	${CXX} ../main.o ../loadg.o ../DxLib.o ../sdlgfx/SDL_gfxPrimitives.o \
		-o SyobonAction $(LDFLAGS)

../main.o:../main.cpp
	${CXX} $(CFLAGS) -I./sdlgfx -c ../main.cpp -o ../main.o

../loadg.o:../loadg.cpp
	${CXX} $(CFLAGS) -I./sdlgfx -c ../loadg.cpp -o ../loadg.o

../DxLib.o:../DxLib.cpp
	${CXX} $(CFLAGS) -I./sdlgfx -c ../DxLib.cpp -o ../DxLib.o

../sdlgfx/SDL_gfxPrimitives.o:../sdlgfx/SDL_gfxPrimitives.c
	${CC} $(CFLAGS) -I./sdlgfx -c ../sdlgfx/SDL_gfxPrimitives.c -o ../sdlgfx/SDL_gfxPrimitives.o


EXE_FILE = bin/MapJumper
OBJ_FILES = obj/main.o
FLAGS = --std=c++17 -w -fpermissive
CC = g++

exe : clean
exe : $(EXE_FILE)

debug: FLAGS += -ggdb
debug : exe

windows: EXE_FILE = bin/mapJumper.exe
windows: CC = x86_64-w64-mingw32-g++
windows: FLAGS += -lmingw32 -lSDL2main -lSDL2 -mwindows -Wl,--no-undefined -Wl,--dynamicbase -Wl,--nxcompat -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid -static-libgcc
windows: exe

$(EXE_FILE) : $(OBJ_FILES)
	$(CC) -o $(EXE_FILE) $(OBJ_FILES) `pkg-config --libs sdl2 SDL2_image SDL2_ttf SDL2_mixer` -lSDL2main $(FLAGS)

obj/main.o : src/main.cpp src/bounds.h src/player.h src/texture.h src/world.h
	$(CC) -c -o obj/main.o src/main.cpp $(FLAGS)
run : exe
	$(EXE_FILE) &

clean:
	rm -f *.o obj/main.o

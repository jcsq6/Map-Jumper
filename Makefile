EXE_FILE = bin/MapJumper
OBJ_FILES = obj/main.o
FLAGS = --std=c++17 -w

exe : $(EXE_FILE)

debug: FLAGS += -ggdb
debug : exe

windows: EXE_FILE = bin/mapJumper.exe
windows: exe

$(EXE_FILE) : $(OBJ_FILES)
	g++ -o $(EXE_FILE) $(OBJ_FILES) `pkg-config --libs sdl2 SDL2_image SDL2_ttf SDL2_mixer` $(FLAGS)

obj/main.o : src/main.cpp src/bounds.h src/player.h src/texture.h src/world.h
	g++ -c -o obj/main.o src/main.cpp $(FLAGS)

run : exe
	$(EXE_FILE) &

clean:
	rm -f *.o obj/main.o

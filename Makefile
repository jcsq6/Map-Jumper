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
#obj/images.o: assets/Blue_Cube.png assets/Blue_Cube_Fade.png assets/Blue_Jump.png assets/blueSpike.png assets/Neutral_Cube.png assets/Neutral_Jump.png assets/neutralSpike.png assets/Red_Cube.png assets/Red_Cube_Fade.png assets/Red_Jump.png assets/redSpike.png
#	ld  -r -b binary -o obj/images.o assets/Blue_Cube.png assets/Blue_Cube_Fade.png assets/Blue_Jump.png assets/blueSpike.png assets/Neutral_Cube.png assets/Neutral_Jump.png assets/neutralSpike.png assets/Red_Cube.png assets/Red_Cube_Fade.png assets/Red_Jump.png assets/redSpike.png
run : exe
	$(EXE_FILE) &

clean:
	rm -f *.o obj/main.o

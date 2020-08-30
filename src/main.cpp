#include "player.h"
#include <fstream>
#include <filesystem>
#include <vector>

int main(int argc, char* argv[]) {
	int fps = 60;
	int ticksPerFrame = 1000 / fps;

	int frameStrt;

	bool running = true;

	bool isFullscreen = false;

	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_AllocateChannels(2);

	Mix_Volume(0, MIX_MAX_VOLUME / 3);
	Mix_Volume(1, MIX_MAX_VOLUME / 10);

	Mix_Chunk* mapSwitchSound = Mix_LoadWAV("assets/mapSwitch.wav");

	SDL_Window* window = SDL_CreateWindow("MapJumper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_RenderSetLogicalSize(renderer, 1920, 1080);

	std::ifstream lvl;
	json lvlJson;

	std::vector<texture> lvls;
	for (const auto& f : std::filesystem::directory_iterator("lvls")) {
		std::size_t pos = f.path().string().find('\\lvls') + 2;
		std::string lvlName = f.path().string().substr(pos);
		std::string extension = ".json";
		std::string::size_type i = lvlName.find(extension);
		lvlName.erase(i, extension.length());
		lvls.push_back(texture(lvlName.c_str(), 50, renderer));
	}

menu:

	bool inMenu = true;

	int curLvlI = 0;

	while (inMenu) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				inMenu = running = false;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					running = inMenu = false;
					break;
				case SDLK_F11:
					if (!isFullscreen) SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
					else SDL_SetWindowFullscreen(window, NULL);
					isFullscreen = !isFullscreen;
					break;
				case SDLK_RETURN:
					inMenu = false;
					break;
				case SDLK_DOWN:
					curLvlI++;
					curLvlI %= lvls.size();
					break;
				case SDLK_UP:
					curLvlI--;
					if (curLvlI < 0) curLvlI = lvls.size() - 1;
					break;
				}
				break;
			}
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		int height = 50;
		int i = 0;
		for (auto l : lvls) {
			if (i == curLvlI) {
				SDL_Rect arrow = { 1920 / 2 - l.getWidth() - 10, height + l.getHeight() / 2, 20, 10 };
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				SDL_RenderFillRect(renderer, &arrow);
			}
			l.draw(1920 / 2 - l.getWidth() / 2, height, 0);
			height += l.getHeight() + 10;
			i++;
		}

		SDL_RenderPresent(renderer);
	}
	std::string name = "lvls/" + lvls[curLvlI].getText() + ".json";
	lvl = std::ifstream(name);
	lvl >> lvlJson;

beginLvl:

	world mainWorld(1920, 1080, lvlJson, renderer);
	player mainPlayer(&mainWorld, renderer);

	texture background("assets/background.png", renderer, 1920, 1080);

	while (running) {
		frameStrt = SDL_GetTicks();

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					running = false;
					break;
				case SDLK_F11:
					if (!isFullscreen) SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
					else SDL_SetWindowFullscreen(window, NULL);
					isFullscreen = !isFullscreen;
					break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (mainWorld.getMapNum() == 1) mainWorld.setMapNum(2);
				else mainWorld.setMapNum(1);
				Mix_PlayChannel(0, mapSwitchSound, 0);
				break;
			}
		}
		const Uint8* keys = SDL_GetKeyboardState(NULL);
		mainPlayer.update(keys);

		if (mainPlayer.isTouchingSpike()) goto beginLvl;
		if (mainPlayer.isTouchingEnd()) goto menu;

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		background.draw(0, 0, 0);

		mainWorld.draw();

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		mainPlayer.draw();

		SDL_RenderPresent(renderer);

		while (SDL_GetTicks() - frameStrt < ticksPerFrame) {}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
}
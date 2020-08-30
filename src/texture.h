#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>

class texture {
public:
	texture() {}
	texture(const char* filename, SDL_Renderer* rend, int w, int h) {
		renderer = rend;

		IMG_Init(IMG_INIT_PNG);

		surf = IMG_Load(filename);
		textre = SDL_CreateTextureFromSurface(rend, surf);

		width = w;
		height = h;

		srcRect.x = 0;
		srcRect.y = 0;

		srcRect.w = destRect.w = w;
		srcRect.h = destRect.h = h;
	}
	texture(const char* text, int fontSize, SDL_Renderer* rend) {
		renderer = rend;

		textCol = { 0, 0, 0, 0 };

		t = text;

		font = TTF_OpenFont("assets/arial.ttf", fontSize);

		if (font == NULL) {
			std::cout << "font is null: " << SDL_GetError();
		}

		surf = TTF_RenderText_Solid(font, t.c_str(), textCol);
		textre = SDL_CreateTextureFromSurface(renderer, surf);

		SDL_QueryTexture(textre, NULL, NULL, &width, &height);

		srcRect.x = 0;
		srcRect.y = 0;

		srcRect.w = destRect.w = width;
		srcRect.h = destRect.h = height;
	}
	std::string getText() { return t; }
	void draw(int x, int y, int angle) {
		destRect.x = x;
		destRect.y = y;

		srcRect.x = 0;
		srcRect.y = 0;

		SDL_RenderCopyEx(renderer, textre, &srcRect, &destRect, angle, NULL, SDL_FLIP_NONE);
	}
	void drawFrame(int x, int y, int frame, SDL_RendererFlip flip) {
		destRect.x = x;
		destRect.y = y;

		srcRect.x = width * frame;
		srcRect.y = 0;

		SDL_RenderCopyEx(renderer, textre, &srcRect, &destRect, 0, NULL, flip);
	}

	int getWidth() { return width; }
	int getHeight() { return height; }

	SDL_Renderer* getRenderer() { return renderer; }
private:
	SDL_Renderer* renderer;
	SDL_Texture* textre;
	SDL_Surface* surf;

	TTF_Font* font;
	SDL_Color textCol;

	int width;
	int height;

	std::string t;

	SDL_Rect srcRect;
	SDL_Rect destRect;
};
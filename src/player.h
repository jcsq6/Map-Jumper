#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <SDL2/SDL_mixer.h>
#include "bounds.h"
#include "world.h"
#include "../assets/cubeText.xpm"

class player {
public:
	player(world* w1, SDL_Renderer* renderer) {
		w = 40;
		h = 40;
		angle = 0;
		angleInc = 0;
		rend = renderer;
		world1 = w1;
		pos = world1->getSpawn();
		cubeText = texture(cubeText_xpm, renderer, w, h);
		if (pos.x == 0) pos.x = 2;
		if (pos.y == 0) pos.y = 2;
		if (pos.x == world1->getWidth()) pos.x = world1->getWidth() - 2;
		if (pos.y == world1->getHeight()) pos.y = world1->getHeight() - 2;
		vel = glm::vec2(0, 0);
		bound = boundingBox(pos.x, pos.y, w, h);
		isInAir = true;
		spaceIsReleased = true;
		isStuck = false;
		keyIsReleased = true;
		isAtEnd = false;
		touchesSpike = false;
		canWallJump = false;
		sneak = false;
		framesHeld = 0;
		key = nullptr;
		jumpSound = Mix_LoadWAV("assets/jump.wav");
	}
	bool isTouchingSpike() { return touchesSpike; }
	bool isTouchingEnd() { return isAtEnd; }
	void update(const Uint8* keys) {
		if (keys[SDL_SCANCODE_LSHIFT]) sneak = true;
		else sneak = false;
		if (keys[SDL_SCANCODE_D] && keyIsReleased && framesHeld > 5) {
			if (!sneak) vel.x = 10;
			else vel.x = 3;
		}
		else if (keys[SDL_SCANCODE_A] && keyIsReleased && framesHeld > 5) {
			if (!sneak) vel.x = -10;
			else vel.x = -3;
		}
		else if (keys[SDL_SCANCODE_D] && keyIsReleased && framesHeld <= 5) {
			framesHeld++;
			if (sneak) vel.x = 1;
			else vel.x = framesHeld;
			
		}
		else if (keys[SDL_SCANCODE_A] && keyIsReleased && framesHeld <= 5) {
			framesHeld++;
			if (sneak) vel.x = -1;
			else vel.x = -framesHeld;
		}
		else {
			framesHeld = 0;
			if (isInAir && keyIsReleased) {
				if (vel.x == -10) vel.x = -1;
				else if (vel.x == 10) vel.x = 1;
			}
			else if(keyIsReleased){
				vel.x = 0;
			}
		}
		if (keys[SDL_SCANCODE_SPACE] && (!isInAir || canWallJump) && spaceIsReleased) {
  			if (vel.x < 0) angleInc = -10;
			else angleInc = 10;
			if (canWallJump) {
				if (vel.x < 0) {
					vel.x = 10;
					angleInc = 10;
				}
				else {
					vel.x = -10;
					angleInc = -10;
				}
				if (keys[SDL_SCANCODE_D]) {
					key = new SDL_Scancode;
					*key = SDL_SCANCODE_D;
				}
				else if (keys[SDL_SCANCODE_A]) {
					key = new SDL_Scancode;
					*key = SDL_SCANCODE_A;
				}
			}
			vel.y = -20;
			Mix_PlayChannel(1, jumpSound, 0);
		}
		if (!keys[SDL_SCANCODE_SPACE]) {
			spaceIsReleased = true;
		}
		else if (keys[SDL_SCANCODE_SPACE]) {
			spaceIsReleased = false;
		}

		if (key) {
			if (!keys[*key]) {
				keyIsReleased = true;
				free(key);
				key = nullptr;
			}
			else if (keys[*key]) {
				keyIsReleased = false;
			}
		}
		
		cube cubeTouching = boxVworld(bound + glm::vec2(vel.x, 0), *world1);

		if (cubeTouching == blueJump || cubeTouching == redJump || cubeTouching == neutralJump) {
			canWallJump = true;
		}
		else {
			canWallJump = false;
		}

		touchesSpike = false;
		if ((cubeTouching == neutralSpike || cubeTouching == redSpike || cubeTouching == blueSpike) && !sneak) touchesSpike = true;


		if ((cubeTouching != nothing && !isStuck) || (isStuck && (cubeTouching == neutral || cubeTouching == neutralSpike))) {
			vel.x = 0;
		}

		std::vector<point> pointsTouching = getPointsTouchingWorld(bound + glm::vec2(0, vel.y), *world1);

		cubeTouching = boxVworld(bound + glm::vec2(0, vel.y), *world1);

		if (cubeTouching == neutralSpike || cubeTouching == redSpike || cubeTouching == blueSpike) touchesSpike = true;

		if ((cubeTouching != nothing && !isStuck) || (isStuck && (cubeTouching == neutral || cubeTouching == neutralSpike))) {
			vel.y = 0;
			if (std::find(pointsTouching.begin(), pointsTouching.end(), bottomL) != pointsTouching.end() && std::find(pointsTouching.begin(), pointsTouching.end(), bottomR) != pointsTouching.end()) isInAir = false;
		}

		if (std::find(pointsTouching.begin(), pointsTouching.end(), bottomL) == pointsTouching.end() && std::find(pointsTouching.begin(), pointsTouching.end(), bottomR) == pointsTouching.end() && !isStuck) {
			isInAir = true;
			angle += angleInc;
		}

		if (!isInAir && abs(angle) % 90 != 0) {
			angle += angleInc;
		}

		if (pos.x + vel.x + w < world1->getWidth() && pos.x + vel.x > 0) pos.x += vel.x;
		if (pos.y + vel.y + h < world1->getHeight() && pos.y + vel.y > 0) pos.y += vel.y;

		bound.updatePos(pos);

		bool wasStuck = isStuck;
		isStuck = false;

		if (boxVbox(bound, boundingBox(world1->getEnd().x, world1->getEnd().y, 64, 64))) isAtEnd = true;

		cubeTouching = boxVworld(bound, *world1);
		if (cubeTouching == neutralSpike || cubeTouching == redSpike || cubeTouching == blueSpike) touchesSpike = true;
		if (wasStuck && cubeTouching != nothing) {
			isStuck = true;
		}
		else if (!wasStuck && cubeTouching != nothing) {
			pos -= vel;
			bound = bound - vel;
			if (vel.x == 0 && vel.y == 0) {
				isStuck = true;
			}
		}

		vel += glm::vec2(0, 1);
	}
	void draw() {
		cubeText.draw(pos.x, pos.y, angle);
	}
private:
	glm::vec2 pos;
	glm::vec2 vel;
	world* world1;
	boundingBox bound;
	int w, h;
	int angle, angleInc;
	int framesHeld;
	bool isInAir, isStuck, touchesSpike, isAtEnd, spaceIsReleased, canWallJump, keyIsReleased, sneak;
	SDL_Scancode* key;
	Mix_Chunk* jumpSound;
	SDL_Renderer* rend;
	texture cubeText;
};
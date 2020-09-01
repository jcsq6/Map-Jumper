#pragma once
#include "bounds.h"
#include "texture.h"
#include <iostream>
#include "json.hpp"
#include <vector>
#include <utility>
#include <string_view>
#include "../assets/Blue_Cube.xpm"
#include "../assets/Blue_Cube_Fade.xpm"
#include "../assets/blueSpike.xpm"
#include "../assets/blueSpikeFade.xpm"
#include "../assets/Blue_Jump.xpm"
#include "../assets/Red_Cube.xpm"
#include "../assets/Red_Cube_Fade.xpm"
#include "../assets/redSpike.xpm"
#include "../assets/redSpikeFade.xpm"
#include "../assets/Red_Jump.xpm"
#include "../assets/Neutral_Cube.xpm"
#include "../assets/Neutral_Jump.xpm"
#include "../assets/neutralSpike.xpm"



enum cube {
	blue,
	red,
	neutral,
	blueSpike,
	redSpike,
	neutralSpike,
	redJump,
	blueJump,
	neutralJump,
	worldBounds,
	nothing
};

using json = nlohmann::json;

class world {
public:
	world(int width, int height, json data, SDL_Renderer* renderer) {
		mapNum = 1;

		startX = data["Spawn_Loc"][0];
		startY = data["Spawn_Loc"][1];

		endX = data["End_Loc"][0];
		endY = data["End_Loc"][1];

		w = width;
		h = height;

		blueText = texture(Blue_Cube_xpm, renderer, 64, 64);
		blueFadeText = texture(Blue_Cube_Fade_xpm, renderer, 64, 64);
		blueSpikeText = texture(blueSpike_xpm, renderer, 64, 64);
		blueSpikeFadeText = texture(blueSpikeFade_xpm, renderer, 64, 64);
		redText = texture(Red_Cube_xpm, renderer, 64, 64);
		redFadeText = texture(Red_Cube_Fade_xpm, renderer, 64, 64);
		redSpikeText = texture(redSpike_xpm, renderer, 64, 64);
		redSpikeFadeText = texture(redSpikeFade_xpm, renderer, 64, 64);
		neutralText = texture(Neutral_Cube_xpm, renderer, 64, 64);
		neutralSpikeText = texture(neutralSpike_xpm, renderer, 64, 64);
		neutralJumpText = texture(Neutral_Jump_xpm, renderer, 64, 64);
		redJumpText = texture(Red_Jump_xpm, renderer, 64, 64);
		blueJumpText = texture(Blue_Jump_xpm, renderer, 64, 64);

		limits.push_back(boundingBox(-20, 0, 20, h));
		limits.push_back(boundingBox(w, 0, 20, h));
		limits.push_back(boundingBox(0, -20, w, 20));
		limits.push_back(boundingBox(0, h, w, 20));

		for (auto pt : data["map1"]["Blue_Cube"]) {
			map1.push_back(std::make_pair(blue, boundingBox(pt[0], pt[1], 64, 64)));
			map1Rots.push_back(pt[2]);
		}
		for (auto pt : data["map1"]["Neutral_Cube"]) {
			map1.push_back(std::make_pair(neutral, boundingBox(pt[0], pt[1], 64, 64)));
			map1Rots.push_back(pt[2]);
			map2.push_back(std::make_pair(neutral, boundingBox(pt[0], pt[1], 64, 64)));
			map2Rots.push_back(pt[2]);
		}
		for (auto pt : data["map2"]["Red_Cube"]) {
			map2.push_back(std::make_pair(red, boundingBox(pt[0], pt[1], 64, 64)));
			map2Rots.push_back(pt[2]);
		}

		for (auto pt : data["map1"]["Blue_Spike"]) {
			map1.push_back(std::make_pair(blueSpike, boundingBox(pt[0], int(pt[1]) + 32, 64, 32)));
			map1Rots.push_back(pt[2]);
		}
		for (auto pt : data["map1"]["Blue_Jump"]) {
			map1.push_back(std::make_pair(blueJump, boundingBox(pt[0], pt[1], 64, 64)));
			map1Rots.push_back(pt[2]);
		}
		for (auto pt : data["map2"]["Red_Jump"]) {
			map2.push_back(std::make_pair(redJump, boundingBox(pt[0], pt[1], 64, 64)));
			map2Rots.push_back(pt[2]);
		}
		for (auto pt : data["map1"]["Neutral_Spike"]) {
			map1.push_back(std::make_pair(neutralSpike, boundingBox(pt[0], int(pt[1]) + 32, 64, 32)));
			map1Rots.push_back(pt[2]);
			map2.push_back(std::make_pair(neutralSpike, boundingBox(pt[0], int(pt[1]) + 32, 64, 32)));
			map2Rots.push_back(pt[2]);
		}
		for (auto pt : data["map1"]["Neutral_Jump"]) {
			map1.push_back(std::make_pair(neutralJump, boundingBox(pt[0], pt[1], 64, 64)));
			map1Rots.push_back(pt[2]);
			map2.push_back(std::make_pair(neutralJump, boundingBox(pt[0], pt[1], 64, 64)));
			map2Rots.push_back(pt[2]);
		}
		for (auto pt : data["map2"]["Red_Spike"]) {
			map2.push_back(std::make_pair(redSpike, boundingBox(pt[0], int(pt[1]) + 32, 64, 32)));
			map2Rots.push_back(pt[2]);
		}
	}
	void setMapNum(int num) {
		if (num == 1 || num == 2) mapNum = num;
	}
	int getMapNum() { return mapNum; }
	int getWidth() { return w; }
	int getHeight() { return h; }
	glm::vec2 getSpawn() { return glm::vec2(startX, startY); }
	glm::vec2 getEnd() { return glm::vec2(endX, endY); }
	void draw() {
		int i = 0;
		for (auto block : map1) {
			if (mapNum == 1) {
				if (block.first == blue) blueText.draw(block.second.getSDLrect().x, block.second.getSDLrect().y, map1Rots[i]);
				else if (block.first == blueSpike) blueSpikeText.draw(block.second.getSDLrect().x, block.second.getSDLrect().y - 32, map1Rots[i]);
				else if(block.first == blueJump) blueJumpText.draw(block.second.getSDLrect().x, block.second.getSDLrect().y, map1Rots[i]);
			}
			else if (mapNum == 2) {
				if (block.first == blue || block.first == blueJump) blueFadeText.draw(block.second.getSDLrect().x, block.second.getSDLrect().y, map1Rots[i]);
				else if (block.first == blueSpike) blueSpikeFadeText.draw(block.second.getSDLrect().x, block.second.getSDLrect().y - 32, map1Rots[i]);
			}
			i++;
		}
		i = 0;
		for (auto block : map2) {
			if (mapNum == 2) {
				if (block.first == red) redText.draw(block.second.getSDLrect().x, block.second.getSDLrect().y, map2Rots[i]);
				else if (block.first == redSpike) redSpikeText.draw(block.second.getSDLrect().x, block.second.getSDLrect().y - 32, map2Rots[i]);
				else if (block.first == redJump) redJumpText.draw(block.second.getSDLrect().x, block.second.getSDLrect().y, map1Rots[i]);
			}
			else if (mapNum == 1) {
				if (block.first == red || block.first == redJump) redFadeText.draw(block.second.getSDLrect().x, block.second.getSDLrect().y, map2Rots[i]);
				else if (block.first == redSpike) redSpikeFadeText.draw(block.second.getSDLrect().x, block.second.getSDLrect().y - 32, map2Rots[i]);
			}
			if (block.first == neutral) neutralText.draw(block.second.getSDLrect().x, block.second.getSDLrect().y, map2Rots[i]);
			else if (block.first == neutralSpike) neutralSpikeText.draw(block.second.getSDLrect().x, block.second.getSDLrect().y - 32, map2Rots[i]);
			else if (block.first == neutralJump) neutralJumpText.draw(block.second.getSDLrect().x, block.second.getSDLrect().y, map1Rots[i]);
			i++;
		}
	}
private:
	std::vector<boundingBox> limits;
	std::vector<std::pair<cube, boundingBox>> map1;
	std::vector<std::pair<cube, boundingBox>> map2;
	std::vector<int> map1Rots;
	std::vector<int> map2Rots;
	texture blueText;
	texture blueFadeText;
	texture blueSpikeText;
	texture blueSpikeFadeText;
	texture redText;
	texture redFadeText;
	texture redSpikeText;
	texture redSpikeFadeText;
	texture neutralText;
	texture neutralSpikeText;
	texture redJumpText;
	texture blueJumpText;
	texture neutralJumpText;
	int w, h;
	int startX, startY;
	int endX, endY;
	int mapNum;
	friend cube boxVworld(boundingBox b, world w);
	friend std::vector<point> getPointsTouchingWorld(boundingBox box1, world w);
};

cube boxVworld(boundingBox b, world w) {
	cube c = nothing;
	for (auto limit : w.limits) {
		if (boxVbox(b, limit)) return worldBounds;
	}
	if (w.mapNum == 1) {
		for (auto box : w.map1) {
			if (boxVbox(b, box.second)) {
				if (box.first == neutral) return box.first;
				else c = box.first;
			}
		}
	}
	else if (w.mapNum == 2) {
		for (auto box : w.map2) {
			if (boxVbox(b, box.second)) {
				if (box.first == neutral) return box.first;
				else c = box.first;
			}
		}
	}
	return c;
}

std::vector<point> getPointsTouchingWorld(boundingBox box1, world w) {
	std::vector<point> pointsTouchingWorld;
	glm::vec2 topLeft(box1.getSDLrect().x, box1.getSDLrect().y);
	glm::vec2 topRight(box1.getSDLrect().x + box1.getSDLrect().w, box1.getSDLrect().y);
	glm::vec2 bottomLeft(box1.getSDLrect().x, box1.getSDLrect().y + box1.getSDLrect().h);
	glm::vec2 bottomRight(box1.getSDLrect().x + box1.getSDLrect().w, box1.getSDLrect().y + box1.getSDLrect().h);

	for (auto limit : w.limits) {
		if (boxVpt(limit, topLeft)) pointsTouchingWorld.push_back(topL);
		if (boxVpt(limit, topRight)) pointsTouchingWorld.push_back(topR);
		if (boxVpt(limit, bottomLeft)) pointsTouchingWorld.push_back(bottomL);
		if (boxVpt(limit, bottomRight)) pointsTouchingWorld.push_back(bottomR);
	}

	if (w.mapNum == 1) {
		for (auto box : w.map1) {
			if (boxVpt(box.second, topLeft)) pointsTouchingWorld.push_back(topL);
			if (boxVpt(box.second, topRight)) pointsTouchingWorld.push_back(topR);
			if (boxVpt(box.second, bottomLeft)) pointsTouchingWorld.push_back(bottomL);
			if (boxVpt(box.second, bottomRight)) pointsTouchingWorld.push_back(bottomR);
		}
	}
	else if (w.mapNum == 2) {
		for (auto box : w.map2) {
			if (boxVpt(box.second, topLeft)) pointsTouchingWorld.push_back(topL);
			if (boxVpt(box.second, topRight)) pointsTouchingWorld.push_back(topR);
			if (boxVpt(box.second, bottomLeft)) pointsTouchingWorld.push_back(bottomL);
			if (boxVpt(box.second, bottomRight)) pointsTouchingWorld.push_back(bottomR);
		}
	}
	return pointsTouchingWorld;
}
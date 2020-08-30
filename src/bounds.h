#pragma once
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <vector>

enum point {
	topL,
	topR,
	bottomL,
	bottomR
};

class boundingBox {
public:
	boundingBox() {
		min = glm::vec2(0, 0);
		max = glm::vec2(0, 0);
		w = 0;
		h = 0;
	}
	boundingBox(float x, float y, float width, float height) {
		min = glm::vec2(x, y);
		max = glm::vec2(x + width, y + height);
		w = width;
		h = height;
	}
	SDL_Rect getSDLrect() {
		static SDL_Rect r;
		r.x = min.x;
		r.y = min.y;
		r.w = w;
		r.h = h;
		return r;
	}
	void updatePos(glm::vec2 pos) {
		min = pos;
		max = glm::vec2(pos.x + w, pos.y + h);
	}
	boundingBox operator+(glm::vec2 p) {
		return boundingBox(min.x + p.x, min.y + p.y, w, h);
	}
	boundingBox operator-(glm::vec2 p) {
		return boundingBox(min.x - p.x, min.y - p.y, w, h);
	}
private:
	glm::vec2 min;
	glm::vec2 max;
	float w;
	float h;
	friend bool boxVpt(boundingBox box, glm::vec2 pt);
	friend bool boxVbox(boundingBox box1, boundingBox box2);
};

bool boxVpt(boundingBox box, glm::vec2 pt) {
	return (pt.x >= box.min.x && pt.x <= box.max.x && pt.y >= box.min.y && pt.y <= box.max.y);
}

bool boxVbox(boundingBox box1, boundingBox box2) {
	return box1.min.x <= box2.max.x && box1.max.x >= box2.min.x && box1.min.y <= box2.max.y && box1.max.y >= box2.min.y;
}
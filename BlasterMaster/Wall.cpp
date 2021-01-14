#include "Wall.h"

Wall::Wall() {
	type = WALL;
}

void Wall::Render() {
	RenderBoundingBox();
}

void Wall::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}


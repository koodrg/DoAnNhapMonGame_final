#include "Wall.h"

Wall::Wall() {
	type = WALL;
	ChangeAnimation(WALL_NORMAL);
}

void Wall::ChangeAnimation(STATEOBJECT StateObject) {
	this->StateObject = StateObject;
	AnimationSets* animation_sets = AnimationSets::GetInstance();
	LPANIMATION_SET animationSet = animation_sets->Get(type);
	CurAnimation = animationSet->Get(this->StateObject);
}

void Wall::Render() {
	int alpha = 255;
	CurAnimation->Render(x, y, alpha);

	RenderBoundingBox();
}

void Wall::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x;
	t = y;
	r = x + WALL_BBOX_WIDTH;
	b = y + WALL_BBOX_HEIGHT;
}


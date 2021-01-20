#pragma once
#include "GameObject.h"

class Wall : public GameObject {
public:
	Animation* CurAnimation;
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	Wall();
	void ChangeAnimation(STATEOBJECT StateObject);
};
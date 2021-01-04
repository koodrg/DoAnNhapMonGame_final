#pragma once
#include "Enemy.h"
#include "Sophia.h"

class CJumper : public Enemy
{
public:
	bool is;
	bool isJump;
	Animation* CurAnimation;
	CJumper();
	void ChangeAnimation(STATEOBJECT StateObject);
	void SetType(TYPE type) { this->type = type; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void Reset();
};
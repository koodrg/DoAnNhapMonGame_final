#pragma once
#include "Enemy.h"
#include "EnemyBullet.h"


class Teleporter : public Enemy
{
public:
	int startX;
	int startY;
	bool isRender = false;
	EnemyBullet* bullet;
	DWORD timeStartAttack = TIME_DEFAULT;
	DWORD timeDamaged = TIME_DEFAULT;
	Animation* CurAnimation;
	Teleporter(float x, float y) {
		width = 24;
		height = 32;
		this->startX = x;
		this->startY = y;
		type = TELEPORTER;
		ChangeAnimation(TELEPORTERS_IDLE);
	}
	void ChangeAnimation(STATEOBJECT StateObject);
	void SetType(TYPE type) { this->type = type; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	//void Reset();
	void Fire();
	Teleporter();
};


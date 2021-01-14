#pragma once
#include "Enemy.h"
#include "EnemyBullet.h"

#define EYEBALL_MOVING_SPEED_X 0.05f
#define EYEBALL_MOVING_SPEED_Y 0.05f

class EyeBall : public Enemy
{
public:
	EnemyBullet* bullet;
	DWORD timeStartAttack = TIME_DEFAULT;
	DWORD timeDamaged = TIME_DEFAULT;
	Animation* CurAnimation;
	EyeBall();
	void ChangeAnimation(STATEOBJECT StateObject);
	void SetType(TYPE type) { this->type = type; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	//void Reset();
	void Fire();
	//void Reset(float x = 67 * BIT, float y = 72 * BIT);
};


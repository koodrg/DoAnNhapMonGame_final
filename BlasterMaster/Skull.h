#pragma once
#include "Enemy.h"
#include "Sophia.h"
#include "WhiteBullet.h"


class CSkull : public Enemy
{
	bool isFirstTimeFire = true;  //kiem tra xem co phai lan dau nha dan hay ko
public:
	EnemyBullet* bullet;
	//vector<EnemyBullet*> listBullets;
	DWORD timeStartAttack = TIME_DEFAULT;
	int ny;
	bool jump;
	/*DWORD timeStartFiring = TIME_DEFAULT;*/
	//int count = 0;
	Animation* CurAnimation;
	CSkull();
	void ChangeAnimation(STATEOBJECT StateObject);
	STATEOBJECT GetStateObject() { return this->StateObject; }
	void SetType(TYPE type) { this->type = type; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void Reset();
	void Fire();
};
#include "Skull.h"
#include "Brick.h"

CSkull::CSkull()
{
	type = SKULL;
	health = 1;
	width = SKULL_BBOX_WIDTH;
	height = SKULL_BBOX_HEIGHT;
	Reset();
}

void CSkull::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y + 0.1; // +0.1 de khoi bi va cham voi brick khi di chuyen;
	right = x + SKULL_BBOX_WIDTH;
	bottom = y + SKULL_BBOX_HEIGHT + 0.1;
}

void CSkull::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt, coObjects);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (health == 0) {
		StateObject = ENEMY_DEAD;
		isDead = true;
	}

	if (StateObject != ENEMY_DEAD)
		CalcPotentialCollisions(coObjects, coEvents);


	if (coEvents.size() == 0)
	{

		float cxm, cym;
		player->GetPosition(cxm, cym);

		float cex4, cey4;
		this->GetPosition(cex4, cey4);

		double kc = sqrt((cex4 - cxm) * (cex4 - cxm) + (cey4 - cym) * (cey4 - cym));

		if (kc <= 150 && jump == false)
		{
			ChangeAnimation(SKULL_STATE_WALKING_LEFT);
			if (this->nx > 0 && cex4 >= cxm && jump == false)
			{
				ChangeAnimation(SKULL_STATE_JUMP_LEFT);
			}
			else if (this->nx < 0 && cex4 <= cxm && jump == false)
			{
				ChangeAnimation(SKULL_STATE_JUMP_RIGHT);
			}
		}

		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.14f;
		y += min_ty * dy + ny * 0.14f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;


		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Brick*>(e->obj)) // if e->obj is Brick
			{
				Brick* brick = dynamic_cast<Brick*>(e->obj);
				if (e->ny > 0)
				{
					jump = true;
					ChangeAnimation(SKULL_STATE_IDLE);
					Fire();
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CSkull::Render()
{
	int alpha = 255;
	CurAnimation->Render(x, y, alpha);
}

void CSkull::ChangeAnimation(STATEOBJECT StateObject) {
	this->StateObject = StateObject;
	AnimationSets* animation_sets = AnimationSets::GetInstance();
	LPANIMATION_SET animationSet = animation_sets->Get(type);
	CurAnimation = animationSet->Get(this->StateObject);
	switch (this->StateObject)
	{
	case SKULL_STATE_IDLE:
		vx = 0;
		vy = 0;
		nx = 1;
		break;
	case SKULL_STATE_JUMP_RIGHT:
		vx = 0;
		vy = -SKULL_JUMP_Y;
		nx = 1;
		break;
	case SKULL_STATE_JUMP_LEFT:
		vx = 0;
		vy = -SKULL_JUMP_Y;
		nx = -1;
		break;
	case SKULL_STATE_WALKING_LEFT:
		vx = -SKULL_WALKING_SPEED;
		nx = -1;
		break;
	case SKULL_STATE_WALKING_RIGHT:
		vx = SKULL_WALKING_SPEED;
		nx = 1;
		break;
	default:
		break;
	}
}

void CSkull::Reset() {
	nx = -1;
	ny = 1;
	jump = false;
	ChangeAnimation(SKULL_STATE_WALKING_RIGHT);
}

void CSkull::Fire() {
	
	if (timeStartAttack == TIME_DEFAULT) {
		timeStartAttack = GetTickCount();
		IsFiring = false;
	}

	if  (isFirstTimeFire && this->StateObject == SKULL_STATE_IDLE) {
		bullet = new EnemyBullet();
		bullet->type = ENEMY_BULLET;
		if (nx > 0) {
			bullet->ChangeAnimation(WHITE_BULLET_MOVING_RIGHT);
			bullet->SetPosition(x + SKULL_BBOX_WIDTH, y + SKULL_BBOX_HEIGHT / 3);
		}
		else {
			bullet->ChangeAnimation(WHITE_BULLET_MOVING_LEFT);
			bullet->SetPosition(x + SKULL_BBOX_WIDTH, y + SKULL_BBOX_HEIGHT / 3);
		}
		//listBullets.push_back(bullet);
		IsFiring = true;
		
		timeStartAttack = TIME_DEFAULT;
		
	}


}

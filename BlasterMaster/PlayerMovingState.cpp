#include "PlayerStandingState.h"
#include "PlayerMovingState.h"
#include "PlayerUpperState.h"
#include "PlayerUpwardMovingState.h"
#include "PlayerTurningState.h"

PlayerMovingState::PlayerMovingState(DWORD timeFinish) {

	if (Allow[SOPHIA]) {
		player->RenderOneFrame = false;
		player->IsMoving = true;
		if (player->nx > 0) { //nx > 0 : right , nx < 0 : left 
			player->StateName = SOPHIA_MOVING_RIGHT;
			player->vx = SOPHIA_MOVING_SPEED;
		}
		else {
			player->StateName = SOPHIA_MOVING_LEFT;
			player->vx = -SOPHIA_MOVING_SPEED;
		}

		player->stateBoundingBox = SOPHIA_BOUNDING_BOX;
	}
	if (Allow[JASON]) {
		playerSmall->RenderOneFrame = false;
		if (playerSmall->nx > 0) {
			playerSmall->StateName = JASON_MOVING_RIGHT;
			playerSmall->vx = JASON_MOVING_SPEED;
		}
		else {
			playerSmall->StateName = JASON_MOVING_LEFT;
			playerSmall->vx = -JASON_MOVING_SPEED;
		}

		playerSmall->stateBoundingBox = JASON_BOUNDING_BOX;
	}
	if (Allow[BIG_JASON]) {
		playerBig->RenderOneFrame = false;
		if (playerBig->nx > 0) {
			playerBig->StateName = BIG_JASON_MOVING_RIGHT;
			playerBig->vx = BIG_JASON_MOVING_SPEED;
			playerBig->vy = 0;
		}
		else if(playerBig->nx < 0) {
			playerBig->StateName = BIG_JASON_MOVING_LEFT;
			playerBig->vx = -BIG_JASON_MOVING_SPEED;
			playerBig->vy = 0;
		}
		else if (playerBig->ny > 0) {
			playerBig->StateName = BIG_JASON_MOVING_UP;
			playerBig->vy = -BIG_JASON_MOVING_SPEED;
			playerBig->vx = 0;
		}
		else if (playerBig->ny < 0) {
			playerBig->StateName = BIG_JASON_MOVING_DOWN;
			playerBig->vy = BIG_JASON_MOVING_SPEED;
			playerBig->vx = 0;
		}
		playerBig->stateBoundingBox = BIG_JASON_BOUNDING_BOX;
	}
}

void PlayerMovingState::Update() {
	
	this->HandleKeyboard();
}

void PlayerMovingState::HandleKeyboard() {
	if (Allow[BIG_JASON]) {
		if (keyCode[DIK_RIGHT] && keyCode[DIK_LEFT]) {
			playerBig->ChangeAnimation(new PlayerStandingState());
		}
		else if (keyCode[DIK_RIGHT] && keyCode[DIK_UP]) {
			if (playerBig->nx > 0 && playerBig->ny == 0) {
				playerBig->ChangeAnimation(new PlayerMovingState());
				playerBig->ny = 0;
				playerBig->nx = 1;
				playerBig->vx = BIG_JASON_MOVING_SPEED;
				playerBig->vy = -BIG_JASON_MOVING_SPEED;
			}
			else if (playerBig->nx == 0 && playerBig->ny > 0) {
				playerBig->ChangeAnimation(new PlayerMovingState());
				playerBig->ny = 1;
				playerBig->nx = 0;
				playerBig->vx = BIG_JASON_MOVING_SPEED;
				playerBig->vy = -BIG_JASON_MOVING_SPEED;
			}
		}
		else if (keyCode[DIK_RIGHT] && keyCode[DIK_DOWN]) {

			if (playerBig->nx > 0 && playerBig->ny == 0) {
				playerBig->ChangeAnimation(new PlayerMovingState());
				playerBig->ny = 0;
				playerBig->nx = 1;
				playerBig->vx = BIG_JASON_MOVING_SPEED;
				playerBig->vy = BIG_JASON_MOVING_SPEED;
			}
			else if (playerBig->nx == 0 && playerBig->ny > 0) {
				playerBig->ChangeAnimation(new PlayerMovingState());
				playerBig->ny = -1;
				playerBig->nx = 0;
				playerBig->vx = BIG_JASON_MOVING_SPEED;
				playerBig->vy = BIG_JASON_MOVING_SPEED;
			}

		}
		else if (keyCode[DIK_LEFT] && keyCode[DIK_UP]) {

			if (playerBig->nx < 0 && playerBig->ny == 0) {
				playerBig->ChangeAnimation(new PlayerMovingState());
				playerBig->ny = 0;
				playerBig->nx = -1;
				playerBig->vx = -BIG_JASON_MOVING_SPEED;
				playerBig->vy = -BIG_JASON_MOVING_SPEED;
			}
			else if (playerBig->nx == 0 && playerBig->ny > 0) {
				playerBig->ChangeAnimation(new PlayerMovingState());
				playerBig->ny = 1;
				playerBig->nx = 0;
				playerBig->vx = -BIG_JASON_MOVING_SPEED;
				playerBig->vy = -BIG_JASON_MOVING_SPEED;
			}
		}
		else if (keyCode[DIK_LEFT] && keyCode[DIK_DOWN]) {
			if (Allow[BIG_JASON]) {
				if (playerBig->nx < 0 && playerBig->ny == 0) {
					playerBig->ChangeAnimation(new PlayerMovingState());
					playerBig->ny = 0;
					playerBig->nx = -1;
					playerBig->vx = -BIG_JASON_MOVING_SPEED;
					playerBig->vy = BIG_JASON_MOVING_SPEED;
				}
				else if (playerBig->nx == 0 && playerBig->ny < 0) {
					playerBig->ChangeAnimation(new PlayerMovingState());
					playerBig->ny = -1;
					playerBig->nx = 0;
					playerBig->vx = -BIG_JASON_MOVING_SPEED;
					playerBig->vy = BIG_JASON_MOVING_SPEED;
				}
			}
		}
		else if (keyCode[DIK_UP]) {
			playerBig->ny = 1;
			playerBig->nx = 0;
			playerBig->ChangeAnimation(new PlayerMovingState());
		}
		else if (keyCode[DIK_DOWN]) {
			playerBig->ny = -1;
			playerBig->nx = 0;
			playerBig->ChangeAnimation(new PlayerMovingState());
		}
		else if (keyCode[DIK_RIGHT]) {
			playerBig->nx = 1;
			playerBig->ny = 0;
			playerBig->ChangeAnimation(new PlayerMovingState());
		}
		else if (keyCode[DIK_LEFT]) {
			playerBig->nx = -1;
			playerBig->ny = 0;
			playerBig->ChangeAnimation(new PlayerMovingState());
		}
		else {
			playerBig->ChangeAnimation(new PlayerStandingState());
		}
	}
	else if (Allow[JASON] || Allow[SOPHIA]) {
		if (keyCode[DIK_RIGHT] && keyCode[DIK_LEFT]) {
			if (Allow[SOPHIA])
				player->ChangeAnimation(new PlayerStandingState(), MOVE_TO_NORMAL);
			else if (Allow[JASON])
				playerSmall->ChangeAnimation(new PlayerStandingState());
		}
		if (keyCode[DIK_UP]) {
			if (Allow[SOPHIA]) {
				if (!player->IsUp)
					player->y = player->y - (SOPHIA_UP_BBOX_HEIGHT - SOPHIA_BBOX_HEIGHT);
				player->IsUp = true;
				player->ChangeAnimation(new PlayerUpperState(), MOVE_TO_NORMAL);
			}
		}
		else if (keyCode[DIK_RIGHT]) {
			if (Allow[SOPHIA]) {
				if (player->nx > 0)
					player->ChangeAnimation(new PlayerMovingState(), NORMAL);
				else
				{
					player->vx = 0;
					if (player->CurAnimation->currentFrame == 0 || player->CurAnimation->currentFrame == 4) {
						player->idFrame = 0;
					}
					else if (player->CurAnimation->currentFrame == 1 || player->CurAnimation->currentFrame == 5) {
						player->idFrame = 1;
					}
					else if (player->CurAnimation->currentFrame == 2 || player->CurAnimation->currentFrame == 6) {
						player->idFrame = 2;
					}
					else if (player->CurAnimation->currentFrame == 3 || player->CurAnimation->currentFrame == 7) {
						player->idFrame = 3;
					}
					player->ChangeAnimation(new PlayerTurningState());
					player->CurAnimation->currentFrame = -1;
					player->CurAnimation->isLastFrame = false;
				}
			}
			else if (Allow[JASON]) {
				playerSmall->nx = 1;
				playerSmall->ChangeAnimation(new PlayerMovingState());
			}
		}
		else if (keyCode[DIK_LEFT]) {
			if (Allow[SOPHIA]) {
				if (player->nx < 0)
					player->ChangeAnimation(new PlayerMovingState(), NORMAL);
				else
				{
					player->vx = 0;
					if (player->CurAnimation->currentFrame == 0 || player->CurAnimation->currentFrame == 4) {
						player->idFrame = 0;
					}
					else if (player->CurAnimation->currentFrame == 1 || player->CurAnimation->currentFrame == 5) {
						player->idFrame = 1;
					}
					else if (player->CurAnimation->currentFrame == 2 || player->CurAnimation->currentFrame == 6) {
						player->idFrame = 2;
					}
					else if (player->CurAnimation->currentFrame == 3 || player->CurAnimation->currentFrame == 7) {
						player->idFrame = 3;
					}
					player->ChangeAnimation(new PlayerTurningState());
					player->CurAnimation->currentFrame = -1;
					player->CurAnimation->isLastFrame = false;
				}
			}
			if (Allow[JASON]) {
				playerSmall->nx = -1;
				playerSmall->ChangeAnimation(new PlayerMovingState());
			}
		}
		else {
			if (Allow[SOPHIA])
				player->ChangeAnimation(new PlayerStandingState(), MOVE_TO_NORMAL);
			if (Allow[JASON])
				playerSmall->ChangeAnimation(new PlayerStandingState());
		}
	}
}
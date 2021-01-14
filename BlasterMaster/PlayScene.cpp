#include <iostream>
#include <fstream>

#include "PlayScene.h"
#include "debug.h"
#include "Textures.h"
#include "Sprites.h"
#include "Map.h"
#include "PlayerStandingState.h"
#include "PlayerMovingState.h"
#include "BulletMovingState.h"

using namespace std;

PlayScene::PlayScene(int id, LPCWSTR filePath) :
	Scene(id, filePath) {
	keyHandler = new PlaySceneKeyHandler(this);
	gameCamera = Camera::GetInstance();
}

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_CLEAR_ANIMATIONS_SETS	6
#define SCENE_SECTION_OBJECTS	7
#define SCENE_SECTION_SWITCH_SCENE		8
#define SCENE_SECTION_MAP	9

#define MAX_SCENE_LINE 1024

#define SPEED 0.25

void PlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	Textures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
	DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);

}

void PlayScene::_ParseSection_MAP(string line) {
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines

	MAP->texID = atoi(tokens[0].c_str());
	MAP->width = atoi(tokens[1].c_str());
	MAP->height = atoi(tokens[2].c_str());

	// create cells
	grid->cols = (MAP->width / 90) + 1;
	grid->rows = (MAP->height / 90) + 1;
	grid->Init();
}

void PlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = Textures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	Sprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void PlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	LPANIMATION ani = new Animation();
	//DebugOut(L"--> %s\n", ToWSTR(line).c_str());
	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	Animations::GetInstance()->Add(ani_id, ani);
}

void PlayScene::_ParseSection_SWITCHSCENE(string line) {
	vector<string> tokens = split(line);
	int id_scene = atoi(tokens[0].c_str());
	Game::GetInstance()->SwitchScene(id_scene);
}

void PlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int types = atoi(tokens[0].c_str());
	int ani_set_id = atoi(tokens[1].c_str());

	TYPE type = static_cast<TYPE>(types);


	if (type == PLAYER)
	{
		TYPE type = static_cast<TYPE>(ani_set_id);
		LPANIMATION_SET set = new AnimationSet();
		for (int i = 2; i < tokens.size(); i += 2)
		{
			int ani_id = atoi(tokens[i].c_str());
			int state = atoi(tokens[i + 1].c_str());
			STATENAME stateName = static_cast<STATENAME>(state);
			set->Add(ani_id, stateName);
		}
		DebugOut(L"--> %s\n", ToWSTR(line).c_str());
		AnimationSets::GetInstance()->Add(type, set);
	}
	else
	{
		TYPE type = static_cast<TYPE>(ani_set_id);
		LPANIMATION_SET set = new AnimationSet();
		for (int i = 2; i < tokens.size(); i += 2)
		{
			int ani_id = atoi(tokens[i].c_str());
			int types = atoi(tokens[i + 1].c_str());
			STATEOBJECT type = static_cast<STATEOBJECT>(types);
			set->Add(ani_id, type);
		}
		DebugOut(L"--> %s\n", ToWSTR(line).c_str());
		AnimationSets::GetInstance()->Add(type, set);
	}
}

void PlayScene::_ParseSection_CLEARANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);
	STATEOBJECT idClear = static_cast<STATEOBJECT>(atoi(tokens[0].c_str()));
	AnimationSets::GetInstance()->ClearAt(idClear);
	DebugOut(L"[INFO] Cleared Animation Set %d!\n", idClear);
}

void PlayScene::_ParseSection_OBJECTS(string line) {
	vector<string> tokens = split(line);
	//(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	TYPE  type = static_cast<TYPE>(object_type);

	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());
	//int id_state = atoi(tokens[3].c_str());
	int width;
	int height;
	x = x * BIT; // change value to 16 bit
	y = y * BIT; // change value to 16 bit

	// General object setup

	GameObject* obj = NULL;
	GameObject* enemy = NULL;
	GameObject* item = NULL;
	AnimationSets* animation_sets = AnimationSets::GetInstance();

	switch (type) {
	case BRICK: 
		width = (int)atoi(tokens[3].c_str()) * BIT;
		height = (int)atoi(tokens[4].c_str()) * BIT;
		obj = new Brick();
		obj->SetPosition(x, y);
		obj->SetSize(width, height);
		grid->LoadObject(obj, x, y);
		break;
	case DAMAGE_BRICK:
		width = (int)atoi(tokens[3].c_str()) * BIT;
		height = (int)atoi(tokens[4].c_str()) * BIT;
		obj = new DamageBrick();
		obj->SetPosition(x, y);
		obj->SetSize(width, height);
		grid->LoadObject(obj, x, y);
		break;
	case STAIR:
		width = (int)atoi(tokens[3].c_str()) * BIT;
		height = (int)atoi(tokens[4].c_str()) * BIT;
		obj = new Stair();
		obj->SetSize(width, height);
		grid->LoadObject(obj, x, y);
		break;
	case PORTAL:
	{
		float width = atof(tokens[3].c_str()) * BIT;
		float height = atof(tokens[4].c_str()) * BIT;
		int scene_id = atoi(tokens[5].c_str());
		obj = new Portal();
		obj->SetSceneId(scene_id);
		obj->SetSize(width, height);
		grid->LoadObject(obj, x, y);
		break;
	}
	case GATE:
	{
		float width = atof(tokens[3].c_str()) * BIT;
		float height = atof(tokens[4].c_str()) * BIT;
		int scene_id = atoi(tokens[5].c_str());
		obj = new Gate();
		obj->SetSceneId(scene_id);
		obj->SetSize(width, height);
		grid->LoadObject(obj, x, y);
		break;
	}
	case WALL:
		width = (int)atoi(tokens[3].c_str()) * BIT;
		height = (int)atoi(tokens[4].c_str()) * BIT;
		obj = new Wall();
		obj->SetPosition(x, y);
		obj->SetSize(width, height);
		grid->LoadObject(obj, x, y);
		break;
	case ORB1:
		enemy = new COrb1();
		grid->LoadObject(enemy, x, y);
		break;
	case WORM:
		enemy = new CWorm();
		grid->LoadObject(enemy, x, y);
		break;
	case FLOATER:
		enemy = new CFloater();
		grid->LoadObject(enemy, x, y);
		break;
	case DOME:
	{
		int state = atoi(tokens[3].c_str());
		enemy = new CDome(state);
		grid->LoadObject(enemy, x, y);
	}
		break;
	case JUMPER:
		enemy = new CJumper();
		grid->LoadObject(enemy, x, y);
		break;
	case INSECT:
		enemy = new CInsect();
		grid->LoadObject(enemy, x, y);
		break;
	case ORB2:
		enemy = new COrb2();
		grid->LoadObject(enemy, x, y);
		break;
	case SKULL:
		enemy = new CSkull();
		grid->LoadObject(enemy, x, y);
		break;
	case MINE:
		enemy = new CMine();
		grid->LoadObject(enemy, x, y);
		break;
	case CANON:
		enemy = new Canon();
		grid->LoadObject(enemy, x, y);
		break;
	case EYEBALL:
		enemy = new EyeBall();
		grid->LoadObject(enemy, x, y);
		break;
	case TELEPORTER:
		enemy = new Teleporter(x,y);
		grid->LoadObject(enemy, x, y);
		break;
	case INTRO:
		Intro = new CIntro();
		Intro->SetPosition(x, y);
		break;
	case END:
	{
		STATEOBJECT state = static_cast<STATEOBJECT>(atoi(tokens[3].c_str()));
		end = new End();
		end->SetPosition(x / BIT, y / BIT);
		end->ChangeAnimation(state);
		listEnds.push_back(end);
	}
		break;
	case END2:
	{
		STATEOBJECT state = static_cast<STATEOBJECT>(atoi(tokens[3].c_str()));
		end2 = new End2();
		end2->SetPosition(x / BIT, y / BIT);
		end2->ChangeAnimation(state);
		listEnd2.push_back(end2);
	}
	break;
	case END3:
	{
		STATEOBJECT state = static_cast<STATEOBJECT>(atoi(tokens[3].c_str()));
		end3 = new End3();
		end3->SetPosition(x / BIT, y / BIT);
		end3->ChangeAnimation(state);
		listEnd3.push_back(end3);
	}
	break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}
}

void PlayScene::Load() {
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);
	ifstream f;
	f.open(sceneFilePath);
	//curren resource section path
	int section = SCENE_SECTION_UNKNOWN;
	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE)) {
		string line(str);
		if (line[0] == '#') continue; // skip comment lines

		if (line == "[TEXTURES]") {
			section = SCENE_SECTION_TEXTURES; continue;
		}
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[SWITCHSCENE]") {
			section = SCENE_SECTION_SWITCH_SCENE; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[CLEARANIMATIONSETS]") {
			section = SCENE_SECTION_CLEAR_ANIMATIONS_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[MAP]") {
			section = SCENE_SECTION_MAP; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		// data section
		switch (section) {
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_CLEAR_ANIMATIONS_SETS: _ParseSection_CLEARANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_SWITCH_SCENE: _ParseSection_SWITCHSCENE(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		}
	}
	f.close();

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);

	if (GAME->current_scene != 100 && GAME->current_scene != 200 && GAME->current_scene != 300 && GAME->current_scene != 400) {
		player->Reset();
		playerSmall->Reset();
		playerSmall->IsRender = false;
		playerBig->Reset();
		playerBig->IsRender = false;
		if (playerBig->scene_gate == 53) {
			CBoss::GetInstance()->Reset();
		}
	}
}

void PlayScene::Update(DWORD dt) {
	// UPDATE INTRO
	if (GAME->current_scene == 100) {
		Intro->Update(dt, &listObjects);
	}
	// UPDATE ENDSCNE 1
	else if (GAME->current_scene == 200) {
		sound->Stop(GSOUND::S_BOSS);
		for (int i = 0; i < listEnds.size() ; i++) {
			if (timeEnd == TIME_DEFAULT) {
				timeEnd = GetTickCount();
			}
			listEnds[i]->Update(dt, &listObjects);
			if (listEnds[i]->GetStateObject() == END2_MOUNTAIN) {
				if (GetTickCount() - timeEnd >= 1700)
				{
					listEnds[i]->vy = 0.008f;
				}
			}
			if (GetTickCount() - timeEnd >= 7000) {
				Game::GetInstance()->SwitchScene(300);
				timeEnd = TIME_DEFAULT;
			}
		}
	}
	// UPDATE ENDSCNE 2
	else if (GAME->current_scene == 300) {
		sound->Play(GSOUND::S_ENDSCENE23, true);
		for (int i = 0; i < listEnd2.size(); i++) {
			listEnd2[i]->Update(dt, &listObjects);
		}
		if (timeEnd == TIME_DEFAULT) {
			timeEnd = GetTickCount();
		}
		if (GetTickCount() - timeEnd >= 2000) {
			movingX += dt * 0.04;
			if (movingX + SCREEN_WIDTH >= 539)
				movingX = 539 - SCREEN_WIDTH;
			camera->SetCamPos(movingX, 0);
			if (GetTickCount() - timeEnd >= 14000) {
				Game::GetInstance()->SwitchScene(400);
				camera->SetCamPos(0, 0);
				timeEnd = TIME_DEFAULT;
			}
		}
	}
	// UPDATE ENDSCENE 3
	else if (GAME->current_scene == 400) {

		for (int i = 0; i < listEnd3.size(); i++) {
			listEnd3[i]->Update(dt, &listObjects);
		}
	}
	//UPDATE PLAYSCENE
	else  {
		listEnemies.clear();
		listItems.clear();
		listObjects.clear();

		// chuan bi vao phong boss
		if ((playerBig->scene_gate == 39) && !playerBig->doneFlash) { 
			sound->Stop(GSOUND::S_MAP);
			sound->Play(GSOUND::S_WARNING, false);
			Game::GetInstance()->isFlashing = true;
			if (timeEnter == TIME_DEFAULT)
				timeEnter = GetTickCount();
			if (GetTickCount() - timeEnter >= 6000) {
				Game::GetInstance()->isFlashing = false;
				playerBig->doneFlash = true;
				playerBig->scene_gate = 53;
				playerBig->ChangeScene(playerBig->scene_gate);
			}
		}

		//add item vao scene 10
		if (playerBig->scene_gate == 10 && item == 0) {
			thunder = new Thunder();
			thunder->SetPosition(85 * BIT, 50 * BIT);
			hover = new Hover();
			hover->SetPosition(86 * BIT, 50 * BIT);
			grid->AddMovingObject(thunder);
			grid->AddMovingObject(hover);
			item++;
		}


		if (CBoss::GetInstance()->isWakingUp && playerBig->scene_gate == 53) {
			if (timeBoss == TIME_DEFAULT)
				timeBoss = GetTickCount();
			if (GetTickCount() - timeBoss >= 500) {
				sound->Stop(GSOUND::S_WARNING);
				sound->Play(GSOUND::S_BOSS, true);
				CBoss::GetInstance()->isWakingUp = false;
			}
		}

		grid->UpdateCell();
		grid->CalcObjectInViewPort();

		for (auto& obj : grid->GetStaticObjectInViewPort())
		{
			listObjects.push_back(obj);
		}

		for (auto& obj : grid->GetMovingObjectInViewPort())
		{
			switch (obj->type) {
			case ORB1:
			case WORM:
			case FLOATER:
			case DOME:
			case JUMPER:
			case INSECT:
			case ORB2:
			case MINE:
			case SKULL:
			case CANON:
			case EYEBALL:
			case TELEPORTER:
				listEnemies.push_back(static_cast<Enemy*>(obj));
				break;
			case ITEM:
				listItems.push_back(static_cast<Item*>(obj));
				break;
			}
		}

		// Update player
		player->Update(dt, &listObjects, &listEnemies, &listItems, &listEnemyBullets);
		playerSmall->Update(dt, &listObjects, &listEnemies, &listItems, &listEnemyBullets);
		playerBig->Update(dt, &listObjects, &listEnemies, &listItems, &listEnemyBullets);
		// Update boss 
		if (playerBig->scene_gate == 53) {
			CBoss::GetInstance()->IsRender = true;
			CBoss::GetInstance()->Update(dt, &listObjects);
		}

		if (boss->bullet!=NULL)
			listEnemyBullets.push_back(boss->bullet);

		// Update item and enemy
		for (size_t i = 0; i < listEnemies.size(); i++) {
			listEnemies[i]->Update(dt, &listObjects);
			if (listEnemies[i]->GetType() == FLOATER && listEnemies[i]->IsFiring == true) {
				CFloater* floater = static_cast<CFloater*>(listEnemies[i]);
				listEnemyBullets.push_back(floater->bullet);
			}
			if (listEnemies[i]->GetType() == SKULL) {
				CSkull* skull = static_cast<CSkull*>(listEnemies[i]);
				if (skull->bullet && (skull->isFirstTimeFire && skull->IsFiring)) {
					listEnemyBullets.push_back(skull->bullet);
					listEnemies[i]->IsFiring = false;
				}
			}
			if (listEnemies[i]->GetType() == EYEBALL && listEnemies[i]->IsFiring == true) {
				EyeBall* eyeball = static_cast<EyeBall*>(listEnemies[i]);
				listEnemyBullets.push_back(eyeball->bullet);
			}
			if (listEnemies[i]->GetType() == TELEPORTER && listEnemies[i]->IsFiring == true) {
				Teleporter* teleporter = static_cast<Teleporter*>(listEnemies[i]);
				listEnemyBullets.push_back(teleporter->bullet);
			}
			if (listEnemies[i]->GetType() == CANON && listEnemies[i]->IsFiring == true) {
				Canon* canon = static_cast<Canon*>(listEnemies[i]);
				if (canon->bullet1)
					listEnemyBullets.push_back(canon->bullet1);
				if (canon->bullet2)
					listEnemyBullets.push_back(canon->bullet2);
				if (canon->bullet3)
					listEnemyBullets.push_back(canon->bullet3);
				if (canon->bullet4)
					listEnemyBullets.push_back(canon->bullet4);
			}
		}
		for (size_t i = 0; i < listItems.size(); i++) {
			listItems[i]->Update(dt, &listObjects);
		}
#pragma region MyRegion
		// create bullet
		// SOPHIA
		if (player->IsFiring && GetTickCount() - player->timeStartAttack >= 180) {
			if (listBullets.size() < 3) {
				listBullets.push_back(player->bullet);
			}
		}
		// JASON
		if (playerSmall->IsFiring && GetTickCount() - playerSmall->timeStartAttack >= 180) {
			if (listBullets.size() < 3) {
				listBullets.push_back(playerSmall->bullet);
			}
		}
		//BIG JASON
		if (playerBig->IsFiring && GetTickCount() - playerBig->timeStartAttack >= 180) {
			if (listBullets.size() < 3) {
				listBullets.push_back(playerBig->bullet);
			}
		}
		// update bullet
		UpdateBullet(dt);
		// delete enemy
		for (size_t i = 0; i < listEnemies.size(); i++)
		{
			if (listEnemies[i]->StateObject == ENEMY_DEAD) {
				if (randomItem()) {
					power = new Power();
					power->SetPosition(listEnemies[i]->x, listEnemies[i]->y);
					grid->AddMovingObject(power);
				}
				if (listEnemies[i]->type == MINE) {
					EnemyBullet* bullet1 = new EnemyBullet();
					bullet1->type = MINE_BULLET;
					bullet1->ChangeAnimation(MINE_BULLET_JUMPING_LEFT);
					bullet1->SetPosition(listEnemies[i]->x, listEnemies[i]->y);
					EnemyBullet* bullet2 = new EnemyBullet();
					bullet2->type = MINE_BULLET;
					bullet2->ChangeAnimation(MINE_BULLET_JUMPING_LEFT);
					bullet2->SetPosition(listEnemies[i]->x + MINE_BBOX_WIDTH / 3, listEnemies[i]->y + MINE_BBOX_HEIGHT / 3);
					EnemyBullet* bullet3 = new EnemyBullet();
					bullet3->type = MINE_BULLET;
					bullet3->ChangeAnimation(MINE_BULLET_JUMPING_RIGHT);
					bullet3->SetPosition(listEnemies[i]->x + MINE_BBOX_WIDTH / 3, listEnemies[i]->y + MINE_BBOX_HEIGHT / 3);
					listEnemyBullets.push_back(bullet1);
					listEnemyBullets.push_back(bullet2);
					listEnemyBullets.push_back(bullet3);

				}
				listEnemies.erase(listEnemies.begin() + i);
			}
		}
		//delete Item
		for (size_t i = 0; i < listItems.size(); i++)
		{
			if (listItems[i]->isDead == true) {
				listItems.erase(listItems.begin() + i);
			}
		}
#pragma endregion
		// Update camera to follow player
		ChangeScene();

		if (!gameCamera->isChangingMap && !gameCamera->isInTransition) {
			gameCamera->Update();
		}
		else if (gameCamera->isChangingMap) {
			if (Allow[SOPHIA]) {
				float start_map = 0, end_map = 0;
				if (player->scene_id == 1) {
					start_map = START_FIRST_SCENE;
					end_map = END_FIRST_SCENE;
				}
				if (player->scene_id == 2) {
					start_map = START_SECOND_SCENE;
					end_map = END_SECOND_SCENE;
				}
				if (player->scene_id == 3) {
					start_map = START_3RD_SCENE;
					end_map = END_3RD_SCENE;
				}
				if (player->scene_id == 4) {
					start_map = START_4TH_SCENE;
					end_map = END_4TH_SCENE;
				}
				if (player->scene_id == 5) {
					start_map = START_5TH_SCENE;
					end_map = END_5TH_SCENE;
				}
				if (player->scene_id == 6) {
					start_map = START_6TH_SCENE;
					end_map = END_6TH_SCENE;
				}
				if (player->scene_id == 7) {
					start_map = START_7TH_SCENE;
					end_map = END_7TH_SCENE;
				}
				if (player->scene_id == 8) {
					start_map = START_8TH_SCENE;
					end_map = END_8TH_SCENE;
				}
				if (player->scene_id == 9) {
					start_map = START_9TH_SCENE;
					end_map = END_9TH_SCENE;
				}
				if (player->nx < 0) {

					if ((gameCamera->GetCamPosX() + SCREEN_WIDTH) > end_map) {
						gameCamera->SetCamPos(gameCamera->GetCamPosX() - 0.15 * dt, gameCamera->GetCamPosY());
						player->IsRender = false;
					}
					else {
						gameCamera->isChangingMap = false;
						player->IsRender = true;
					}
				}
				else {
					if (gameCamera->GetCamPosX() < start_map) {
						gameCamera->SetCamPos(gameCamera->GetCamPosX() + 0.15 * dt, gameCamera->GetCamPosY());
						player->IsRender = false;
					}
					else {
						gameCamera->isChangingMap = false;
						player->IsRender = true;
					}
				}
			}
			else if (Allow[JASON]) {
				float start_map = 0, end_map = 0;
				if (playerSmall->scene_id == 1) {
					start_map = START_FIRST_SCENE;
					end_map = END_FIRST_SCENE;
				}
				if (playerSmall->scene_id == 2) {
					start_map = START_SECOND_SCENE;
					end_map = END_SECOND_SCENE;
				}
				if (playerSmall->scene_id == 3) {
					start_map = START_3RD_SCENE;
					end_map = END_3RD_SCENE;
				}
				if (playerSmall->scene_id == 4) {
					start_map = START_4TH_SCENE;
					end_map = END_4TH_SCENE;
				}
				if (playerSmall->scene_id == 5) {
					start_map = START_5TH_SCENE;
					end_map = END_5TH_SCENE;
				}
				if (playerSmall->scene_id == 6) {
					start_map = START_6TH_SCENE;
					end_map = END_6TH_SCENE;
				}
				if (playerSmall->scene_id == 7) {
					start_map = START_7TH_SCENE;
					end_map = END_7TH_SCENE;
				}
				if (playerSmall->scene_id == 8) {
					start_map = START_8TH_SCENE;
					end_map = END_8TH_SCENE;
				}
				if (playerSmall->scene_id == 9) {
					start_map = START_9TH_SCENE;
					end_map = END_9TH_SCENE;
				}

				if (playerSmall->nx < 0) {
					if ((gameCamera->GetCamPosX() + SCREEN_WIDTH) > end_map) {
						gameCamera->SetCamPos(gameCamera->GetCamPosX() - 0.15 * dt, gameCamera->GetCamPosY());
						playerSmall->IsRender = false;
					}
					else {
						gameCamera->isChangingMap = false;
						playerSmall->IsRender = true;
					}
				}
				else {
					if (gameCamera->GetCamPosX() < start_map) {
						gameCamera->SetCamPos(gameCamera->GetCamPosX() + 0.15 * dt, gameCamera->GetCamPosY());
						playerSmall->IsRender = false;
					}
					else {
						gameCamera->isChangingMap = false;
						playerSmall->IsRender = true;
					}
				}
			}
		}
		else if (gameCamera->isInTransition) {
			if (playerBig->nx < 0) {

				if ((gameCamera->camPosX + SCREEN_WIDTH / 1.3555555555) > playerBig->x) {
					gameCamera->SetCamPos(gameCamera->camPosX - SPEED * dt, gameCamera->camPosY);
				}
				else {
					Camera::GetInstance()->isInTransition = false;
				}
			}
			else if (playerBig->nx > 0) {
				if ((gameCamera->camPosX + SCREEN_WIDTH / 8) < playerBig->x) {
					gameCamera->SetCamPos(gameCamera->camPosX + SPEED * dt, gameCamera->camPosY);
				}
				else {
					gameCamera->isInTransition = false;
				}
			}
			else if (playerBig->ny < 0) {
				if ((gameCamera->camPosY + SCREEN_HEIGHT / 8) < playerBig->y) {
					gameCamera->SetCamPos(gameCamera->camPosX, gameCamera->camPosY + SPEED * dt);
				}
				else {
					Camera::GetInstance()->isInTransition = false;
				}
			}
			else if (playerBig->ny > 0) {
				if ((gameCamera->camPosY + SCREEN_HEIGHT / 2) > playerBig->y) {
					gameCamera->SetCamPos(gameCamera->camPosX, gameCamera->camPosY - SPEED * dt);
					//DebugOut(L"%d  ", gameCamera->camPosX);
				}
				else {
					Camera::GetInstance()->isInTransition = false;
				}
			}
		}
		hud->Update();
		
		if (Allow[SOPHIA] || Allow[JASON])
		{
			sound->Play(GSOUND::S_MAP, true);
		}

		if (boss->isDead) {
			if (timeEnd == TIME_DEFAULT)
				timeEnd = GetTickCount();
			if (GetTickCount() - timeEnd >= 2000) {
				camera->SetCamPos(0, 0);
				timeEnd = TIME_DEFAULT;
				GAME->SwitchScene(200); // outtro
			}
		}
	}
}

void PlayScene::UpdateBullet(DWORD dt) {
	for (int i = 0; i < listBullets.size(); i++) {
		if (Allow[SOPHIA]) {
			if (listBullets[i]->GetX() - player->x >= SCREEN_WIDTH - (player->x - Camera::GetInstance()->GetCamPosX()) || player->x - listBullets[i]->GetX() >= player->x - Camera::GetInstance()->GetCamPosX()) {
				listBullets.erase(listBullets.begin() + i);
			}
			else if (listBullets[i]->GetY() <= Camera::GetInstance()->GetCamPosY()) {
				listBullets.erase(listBullets.begin() + i);
			}
			else if (listBullets[i]->GetStateObject() == BULLET_SMALL_HIT) {
				if (GetTickCount() - listBullets[i]->timeStartCol >= BULLET_TIME_EXPLOSIVE && listBullets[i]->timeStartCol != TIME_DEFAULT) {
					listBullets.erase(listBullets.begin() + i);
				}
			}
		}
		else if (Allow[JASON]) {
			if (listBullets[i]->GetX() - playerSmall->x >= SCREEN_WIDTH - (playerSmall->x - Camera::GetInstance()->GetCamPosX()) || playerSmall->x - listBullets[i]->GetX() >= playerSmall->x - Camera::GetInstance()->GetCamPosX()) {
				listBullets.erase(listBullets.begin() + i);
			}
			else if (listBullets[i]->GetStateObject() == BULLET_SMALL_HIT) {
				if (GetTickCount() - listBullets[i]->timeStartCol >= BULLET_TIME_EXPLOSIVE && listBullets[i]->timeStartCol != TIME_DEFAULT) {
					listBullets.erase(listBullets.begin() + i);
				}
			}
		}
		else if (Allow[BIG_JASON]) {
			if (listBullets[i]->GetX() - playerBig->x >= SCREEN_WIDTH - (playerBig->x - Camera::GetInstance()->GetCamPosX()) || playerBig->x - listBullets[i]->GetX() >= playerBig->x - Camera::GetInstance()->GetCamPosX()) {
				listBullets.erase(listBullets.begin() + i);
			}
			else if (listBullets[i]->GetY() <= Camera::GetInstance()->GetCamPosY() || listBullets[i]->GetY() >= Camera::GetInstance()->GetCamPosY() + SCREEN_HEIGHT) {
				listBullets.erase(listBullets.begin() + i);
			}
			else if (listBullets[i]->GetStateObject() == BIG_JASON_BULLET_HIT) {
				if (GetTickCount() - listBullets[i]->timeStartCol >= BULLET_TIME_EXPLOSIVE && listBullets[i]->timeStartCol != TIME_DEFAULT) {
					listBullets.erase(listBullets.begin() + i);
				}
			}
		}
	}
	for (int i = 0; i < listBullets.size(); i++) {
		listBullets[i]->Update(dt, &listObjects, &listEnemies);
	}

	for (int i = 0; i < listEnemyBullets.size(); i++) {
		listEnemyBullets[i]->Update(dt, &listObjects);
	}
	for (int i = 0; i < listEnemyBullets.size(); i++) {
		if (listEnemyBullets[i]->GetStateObject() == BULLET_SMALL_HIT) {
			if (GetTickCount() - listEnemyBullets[i]->timeStartCol >= BULLET_TIME_EXPLOSIVE && listEnemyBullets[i]->timeStartCol != TIME_DEFAULT) {
			listEnemyBullets.erase(listEnemyBullets.begin() + i);
			}
		}
	}
	for (int i = 0; i < listEnemyBullets.size(); i++) {
		if (listEnemyBullets[i]->y > camera->camPosY + SCREEN_HEIGHT) {
			listEnemyBullets.erase(listEnemyBullets.begin() + i);
		}
	}
}

void PlayScene::ChangeScene() {
	if (player->IsChangeScene) {
		Game::GetInstance()->SwitchScene(player->scene_id);
		player->ChangeScene();
		player->IsChangeScene = false;
	}
	if (playerSmall->IsChangeScene) {
		Game::GetInstance()->SwitchScene(playerSmall->scene_id);
		playerSmall->ChangeScene();
		playerSmall->IsChangeScene = false;
	}
	if (playerBig->IsChangeScene) {
		Game::GetInstance()->SwitchScene(playerBig->scene_gate);
		playerBig->ChangeScene
		(playerBig->scene_gate);
		playerBig->IsChangeScene = false;
	}
}

void PlayScene::Render() {

	// RENDER INTRO
	if (GAME->current_scene == 100) {
		Intro->Render();
	}
	// RENDER ENDSCENE 1
	if (GAME->current_scene == 200) {
		for (int i = 0; i < listEnds.size(); i++) {
			listEnds[i]->Render();
		}
	}
	// RENDER ENDSCENE 2
	if (GAME->current_scene == 300) {
		for (int i = 0; i < listEnd2.size(); i++) {
			listEnd2[i]->Render();
		}
	}
	// RENDER ENDSCENE 3
	if (GAME->current_scene == 400) {
		for (int i = 0; i < listEnd3.size(); i++) {
			listEnd3[i]->Render();
		}
	}

	if (GAME->current_scene != 100 && GAME->current_scene != 200 && GAME->current_scene != 300 && GAME->current_scene != 400) {
		Map::GetInstance()->Render();
		//grid->RenderCell();
		player->Render();
		playerSmall->Render();
		playerBig->Render();

		if (playerBig->scene_gate == 53) {
			CBoss::GetInstance()->Render();
		}

		for (int i = 0; i < listObjects.size(); i++) {
			listObjects[i]->Render();
		}
		for (int i = 0; i < listEnemies.size(); i++) {
			listEnemies[i]->Render();
		}
		for (int i = 0; i < listItems.size(); i++) {
			listItems[i]->Render();
		}
		for (int i = 0; i < listBullets.size(); i++) {
			listBullets[i]->Render();
		}
		for (int i = 0; i < listEnemyBullets.size(); i++) {
			listEnemyBullets[i]->Render();
		}

		
		hud->Render();
	}
}

void PlayScene::Unload() {
	for (int i = 0; i < HolderObjects.size(); i++)
		delete HolderObjects[i];
	for (int i = 0; i < listEnemies.size(); i++) {
		delete listEnemies[i];
	}
	for (int i = 0; i < listItems.size(); i++) {
		delete listItems[i];
	}
	for (int i = 0; i < listBullets.size(); i++) {
		delete listBullets[i];
	}
	for (int i = 0; i < listObjects.size(); i++) {
		delete listObjects[i];
	}
	HolderObjects.clear();
	listEnemies.clear();
	listItems.clear();
	listObjects.clear();
	grid->GetStaticObjectInViewPort().clear();
	grid->GetMovingObjectInViewPort().clear();
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void PlaySceneKeyHandler::OnKeyDown(int KeyCode){
	switch (KeyCode)
	{
	case DIK_N:
		sound->Stop(GSOUND::S_INTRO);
		Game::GetInstance()->SwitchScene(1);
		break;
	case DIK_M:
		sound->Stop(GSOUND::S_INTRO);
		sound->Stop(GSOUND::S_MAP);
		Game::GetInstance()->SwitchScene(200);
		camera->SetCamPos(0, 0);
		break;
	}

	if (Allow[SOPHIA]){
		keyCode[KeyCode] = true;
		player->OnKeyDown(KeyCode);
	}
	else if (Allow[JASON])
	{
		keyCode[KeyCode] = true;
		playerSmall->OnKeyDown(KeyCode);
	}
	else if (Allow[BIG_JASON])
	{
		keyCode[KeyCode] = true;
		playerBig->OnKeyDown(KeyCode);
	}

	if (player->IsDead) {
		switch (KeyCode) {
		case DIK_A:
			Allow[SOPHIA] = true;
			player->IsRender = true;
			player->Reset();
			break;
		}
	}
}

void PlaySceneKeyHandler::OnKeyUp(int KeyCode) {
	if (Allow[SOPHIA]) {
		keyCode[KeyCode] = false;
		player->OnKeyUp(KeyCode);
	}
	else if (Allow[JASON]) {
		keyCode[KeyCode] = false;
		playerSmall->OnKeyUp(KeyCode);
	}
	else if (Allow[BIG_JASON]) {
		keyCode[KeyCode] = false;
		playerBig->OnKeyUp(KeyCode);
	}
}

void PlaySceneKeyHandler::KeyState(BYTE* states) {

}
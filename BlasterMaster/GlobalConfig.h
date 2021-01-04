#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include <dinput.h>
#include <WinUser.h>
#include <unordered_map>
#include <vector>
#include <iostream> 
#include <string> 
#include <unordered_set> 
#include <map>
#include "debug.h"

using namespace std;


#define TIME_DEFAULT		0
#define TIME_FIRING			200  //???
#define SAFE_DELETE(ptr) \
if(ptr) \
{\
	delete (ptr); \
	ptr = nullptr; \
} \
///// JASON /////
#define JASON_MOVING_SPEED						0.06f
#define JASON_CLIMBING_SPEED					0.1f
#define JASON_JUMP_SPEED_Y						0.35f
#define JASON_GRAVITY							0.001f
#define JASON_JUMP_DEFLECT_SPEED				0.2f
#define JASON_DIE_DEFLECT_SPEED					0.5f

#define JASON_BOUNDING_BOX						114
#define JASON_CRAWLING_BOUNDING_BOX				115

#define JASON_BBOX_WIDTH						10
#define JASON_BBOX_HEIGHT						17

#define JASON_CRAWLING_BBOX_WIDTH				16
#define JASON_CRAWLING_BBOX_HEIGHT				10

///// BIG JASON //////
#define BIG_JASON_MOVING_SPEED						0.08f
#define BIG_JASON_JUMP_SPEED_Y						0.4f
#define BIG_JASON_GRAVITY							0.001f
#define BIG_JASON_JUMP_DEFLECT_SPEED				0.2f
#define BIG_JASON_DIE_DEFLECT_SPEED					0.5f

#define BIG_JASON_BOUNDING_BOX						115

#define BIG_JASON_BBOX_WIDTH						24
#define BIG_JASON_BBOX_HEIGHT						12

///// SOPHIA /////
#define SOPHIA_MOVING_SPEED						0.08f
#define SOPHIA_JUMPING_SPEED_Y					0.37f
#define SOPHIA_GRAVITY							0.001f
#define SOPHIA_JUMPING_DEFLECT_SPEED			0.2f
#define SOPHIA_DIE_DEFLECT_SPEED				0.5f
#define SOPHIA_MOVING_DECLERATION				0.0002f
#define SOPHIA_MOVING_ACCLERATION				0.0002f

#define SOPHIA_BOUNDING_BOX						111
#define SOPHIA_UP_BOUNDING_BOX					112
#define SOPHIA_OPEN_BOUNDING_BOX				113
#define SOPHIA_DEAD_BOUNDING_BOX				110

#define SOPHIA_BBOX_WIDTH						26
#define SOPHIA_BBOX_HEIGHT						18

#define SOPHIA_UP_BBOX_WIDTH					26
#define SOPHIA_UP_BBOX_HEIGHT					34

#define SOPHIA_OPEN_BBOX_WIDTH					26
#define SOPHIA_OPEN_BBOX_HEIGHT					26

#define SOPHIA_DEAD_BBOX_WIDTH					64
#define SOPHIA_DEAD_BBOX_HEIGHT					47
//CAR ANIMATIONS ID //

// STATE CHANGE
#define STAND_TO_MOVE		1
#define MOVE_TO_NORMAL		2
#define NORMAL				3
#define UPWARD_TO_NORMAL	4
#define TURN_TO_NORMAL		5

//ITEM
//POWER
#define ITEM_BBOX_WIDTH						16
#define ITEM_BBOX_HEIGHT					16

//ENEMY
//ORB1
#define ORB1_WALKING_SPEED						0.06f
#define ORB1_WALKING_SPEED_TURN					0.02f

#define ORB1_BBOX_WIDTH							16
#define ORB1_BBOX_HEIGHT						16
#define ORB1_BBOX_HEIGHT_DIE					9
//WORM
#define WORM_WALKING_SPEED						0.04f
#define WORM_WALKING_DROP_X						0.04f
#define WORM_GRAVITY							0.001f

#define WORM_BBOX_WIDTH							18
#define WORM_BBOX_HEIGHT						10
//FLOATER
#define FLOATER_WALKING_SPEED_X					0.05f
#define FLOATER_WALKING_SPEED_Y					0.03f

#define FLOATER_BBOX_WIDTH						16
#define FLOATER_BBOX_HEIGHT						15
#define FLOATER_BBOX_HEIGHT_DIE					9

//JUMPER
#define JUMPER_WALKING_SPEED					0.03f
#define JUMPER_JUMP_X							0.03f
#define JUMPER_JUMP_Y							0.3f
#define JUMPER_GRAVITY							0.02f

#define JUMPER_BBOX_WIDTH						17
#define JUMPER_BBOX_HEIGHT						26
#define JUMPER_BBOX_HEIGHT_DIE					16

//INSECT
#define INSECT_WALKING_SPEED_X					0.01f
#define INSECT_WALKING_SPEED_Y					0.02f
#define INSECT_WALKING_JUMP_Y					0.06f
#define INSECT_WALKING_JUMP_X					0.03f

#define INSECT_BBOX_WIDTH						16
#define INSECT_BBOX_HEIGHT						15
#define INSECT_BBOX_HEIGHT_DIE					9

//ORB2
#define ORB2_WALKING_SPEED						0.1f
#define ORB2_WALKING_TURN_X						0.07f
#define ORB2_WALKING_TURN_Y						0.07f

#define ORB2_BBOX_WIDTH							18
#define ORB2_BBOX_HEIGHT						18
#define OR2B_BBOX_HEIGHT_DIE					 9

//SKULL
#define SKULL_WALKING_SPEED						0.15f
#define SKULL_JUMP_Y							0.1f
#define SKULL_GRAVITY							0.02f

#define SKULL_BBOX_WIDTH						17
#define SKULL_BBOX_HEIGHT						21
#define SKULL_BBOX_HEIGHT_DIE					16

//DOME
#define DOME_WALKING_SPEED						0.07f
#define DOME_GRAVITY_X							0.01f
#define DOME_GRAVITY_Y							0.01f
#define DOME_DROP_FOLLOW						0.1f

#define DOME_BBOX_WIDTH							18
#define DOME_BBOX_HEIGHT						18
#define DOME_BBOX_HEIGHT_DIE					9

//MINE
#define MINE_BBOX_WIDTH							16
#define MINE_BBOX_HEIGHT						10

// CANON
#define CANON_BBOX_WIDTH						26
#define CANON_BBOX_HEIGHT						26

//// SCREEN /////
//#define SCREEN_WIDTH 320
//#define SCREEN_HEIGHT 240

#define SCREEN_WIDTH		270
#define SCREEN_HEIGHT		270

#define TITLE_WIDTH			32
#define TITLE_HEIGHT		32

#define BIT					16
/// variable ///
extern std::unordered_map<int, bool> keyCode;	// manage info keycode have been pressed or not
#define GAME Game::GetInstance()
#define player Sophia::GetInstance()
#define playerSmall Jason::GetInstance()
#define playerBig BigJason::GetInstance()
#define MAP Map::GetInstance()
#define grid Grid::GetInstance()
#define camera Camera::GetInstance()
#define intro CIntro::GetInstance()
#define	hud HUD::GetInstance()
#define boss	CBoss::GetInstance()
#define sound Sound::GetInstance()

extern enum STATENAME {
	///// SOPHIA ///////
	SOPHIA_STANDING_RIGHT,
	SOPHIA_STANDING_LEFT,
	SOPHIA_MOVING_RIGHT,
	SOPHIA_MOVING_LEFT,
	SOPHIA_JUMPING_RIGHT,
	SOPHIA_JUMPING_LEFT,
	SOPHIA_FALLING_RIGHT,
	SOPHIA_FALLING_LEFT,
	SOPHIA_UPWARD_RIGHT_0,
	SOPHIA_UPWARD_RIGHT_1,
	SOPHIA_UPWARD_RIGHT_2,
	SOPHIA_UPWARD_RIGHT_3,
	SOPHIA_UPWARD_LEFT_0,
	SOPHIA_UPWARD_LEFT_1,
	SOPHIA_UPWARD_LEFT_2,
	SOPHIA_UPWARD_LEFT_3,
	SOPHIA_UPWARD_MOVING_RIGHT,
	SOPHIA_UPWARD_MOVING_LEFT,
	SOPHIA_TURN_0,
	SOPHIA_TURN_1,
	SOPHIA_TURN_2,
	SOPHIA_TURN_3,
	SOPHIA_JUMPING_TURN_0,
	SOPHIA_JUMPING_TURN_1,
	SOPHIA_JUMPING_TURN_2,
	SOPHIA_JUMPING_TURN_3,
	SOPHIA_DEAD,
	SOPHIA_OPEN_RIGHT,
	SOPHIA_OPEN_LEFT,
	///// JASON ////////
	JASON_STANDING_RIGHT,
	JASON_STANDING_LEFT,
	JASON_MOVING_RIGHT,
	JASON_MOVING_LEFT,
	JASON_CLIMBING,
	JASON_CRAWLING_RIGHT,
	JASON_CRAWLING_LEFT,
	JASON_JUMPING_RIGHT,
	JASON_JUMPING_LEFT,
	JASON_FALLING_RIGHT,
	JASON_FALLING_LEFT,
	JASON_DEAD_RIGHT,
	JASON_DEAD_LEFT,
	////// BIG JASON ///////
	BIG_JASON_STANDING_RIGHT,
	BIG_JASON_STANDING_LEFT,
	BIG_JASON_STANDING_UP,
	BIG_JASON_STANDING_DOWN,
	BIG_JASON_MOVING_RIGHT,
	BIG_JASON_MOVING_LEFT,
	BIG_JASON_MOVING_UP,
	BIG_JASON_MOVING_DOWN,
	BIG_JASON_DEAD
};

extern enum STATEOBJECT {
	BULLET_SMALL_MOVING_RIGHT,
	BULLET_SMALL_MOVING_LEFT,
	BULLET_SMALL_MOVING_UP,
	BULLET_SMALL_HIT,
	BULLET_BIG_MOVING_RIGHT,
	BULLET_BIG_MOVING_LEFT,
	BULLET_BIG_MOVING_UP,
	BULLET_BIG_HIT,
	BULLET_ELECTRIC,
	BRICK_NORMAL,
	JASON_BULLET_SMALL_MOVING,
	//HUD
	HEALTHBAR_NORMAL,
	HEALTH_NORMAL,
	GUNBAR_NORMAL,
	HOVERBARNULL_NORMAL,
	HOVERBAR_NORMAL,
	//ITEM
	POWER_UP,
	HOVER_UP,
	THUNDER_UP,
	// ENEMY DEAD
	ENEMY_DEAD,
	//// ORB1 /////////
	ORB1_STATE_WALKING_LEFT,
	ORB1_STATE_WALKING_RIGHT,
	ORB1_STATE_TURN_TOP,
	ORB1_STATE_TURN_BOTTOM,
	//// WORM /////
	WORM_STATE_IDLE_RIGHT,
	WORM_STATE_IDLE_LEFT,
	WORM_STATE_WALKING_RIGHT,
	WORM_STATE_WALKING_LEFT,
	WORM_STATE_DROP_RIGHT,
	WORM_STATE_DROP_LEFT,
	//// FLOATER /////
	FLOATER_STATE_WALKING_RIGHT_TOP,
	FLOATER_STATE_WALKING_LEFT_TOP,
	FLOATER_STATE_WALKING_RIGHT_BOTTOM,
	FLOATER_STATE_WALKING_LEFT_BOTTOM,
	// BULLET OF BIG JASON
	BIG_JASON_BULLET_MOVING_RIGHT,
	BIG_JASON_BULLET_MOVING_LEFT,
	BIG_JASON_BULLET_MOVING_UP,
	BIG_JASON_BULLET_MOVING_DOWN,
	BIG_JASON_BULLET_HIT,
	/// GRENADE OF BIG JASON
	BIG_JASON_GRENADE_MOVING,
	BIG_JASON_GRENADE_EXPLOSE,
	//// SKULL /////
	SKULL_STATE_WALKING_RIGHT,
	SKULL_STATE_WALKING_LEFT,
	SKULL_STATE_JUMP_RIGHT,
	SKULL_STATE_JUMP_LEFT,
	SKULL_STATE_IDLE,
	//// ENEMY BULLET ////
	ENEMY_BULLET_SMALL_MOVING,
	/// ENEMY BULLET BIG ////
	ENEMY_BULLET_BIG_MOVING,
	//// JUMPER /////
	JUMPER_STATE_WALKING_RIGHT,
	JUMPER_STATE_WALKING_LEFT,
	JUMPER_STATE_JUMP_RIGHT,
	JUMPER_STATE_JUMP_LEFT,
	JUMPER_STATE_IDLE,
	//// INSECT /////
	INSECT_STATE_WALKING_RIGHT,
	INSECT_STATE_WALKING_LEFT,
	INSECT_STATE_JUMP_RIGHT,
	INSECT_STATE_JUMP_LEFT,
	INSECT_STATE_IDLE,
	//// ORB2 /////
	ORB2_STATE_WALKING_RIGHT,
	ORB2_STATE_WALKING_LEFT,
	ORB2_STATE_TURN_LEFT,
	ORB2_STATE_TURN_RIGHT,
	ORB2_STATE_TURN_BOTTOM,
	ORB2_STATE_EXPLOSE,
	//// MINE ////
	MINE_STATE_IDLE,
	MINE_STATE_DIE,
	//// DOME ////
	DOME_STATE_WALKING_LEFT,
	DOME_STATE_WALKING_TOP,
	DOME_STATE_WALKING_RIGHT,
	DOME_STATE_WALKING_BOTTOM,
	DOME_STATE_IDLE,
	DOME_STATE_DROP,
	DOME_STATE_FOLLOW_RIGHT,
	DOME_STATE_FOLLOW_LEFT,
	// WHITE BULLET //
	WHITE_BULLET_MOVING_RIGHT,
	WHITE_BULLET_MOVING_LEFT,
	// MINE BULLET //
	MINE_BULLET_JUMPING_RIGHT,
	MINE_BULLET_JUMPING_LEFT,
	MINE_BULLET_MOVING_RIGHT,
	MINE_BULLET_MOVING_LEFT,
	/// CANON /////
	CANON_MOVING,
	CANON_NORMAL,
	/// TELEPORTERS /////
	TELEPORTERS_IDLE,
	TELEPORTERS_MOVE,
	// EYE BALL
	EYEBALL_MOVE,
	EYEBALL_IDLE,
	// CANON BULLET //
	CANON_BULLET_MOVING_LEFT,
	CANON_BULLET_MOVING_RIGHT,
	CANON_BULLET_MOVING_DOWN,
	CANON_BULLET_MOVING_UP,
	/// INTRO //////
	INTRO_PLAY,
	/// END SCENE ///
	END1_UP,
	END2_MOUNTAIN,
	END3_DOWN,
	// END 2 ////
	END_HEAD,
	END_BACK,
	// END 3///
	END_BADGE,
	END_LINE,
	// NAME //
	A,
	C,
	E,
	G,
	H,
	I,
	L,
	M,
	N,
	O,
	Q,
	R,
	T,
	U,
	Y,
	//BOSS
	BOSS_MOVE,
	BOSS_LEFT_ARM_NORMAL,
	BOSS_RIGHT_ARM_NORMAL,
	BOSS_LEFT_HAND_NORMAL,
	BOSS_RIGHT_HAND_NORMAL,
	BOSS_BULLET_NORMAL,
	//BOSS BULLET
	BOSS_BULLET,
};

extern enum TYPE {
	PLAYER,
	SOPHIA,
	JASON,
	BIG_JASON,
	BULLET_SMALL,
	BULLET_BIG,
	BRICK,
	PORTAL,
	JASON_BULLET_SMALL,
	STAIR,
	GATE,
	HEALTHBAR,
	HEALTH,
	GUNBAR,
	HOVERBAR_NULL,
	HOVERBAR,
	DAMAGE_BRICK,
	ITEM,
	ORB1,
	WORM,
	FLOATER,
	BIG_JASON_BULLET,
	BIG_JASON_GRENADE,
	DOME,
	ENEMY_BULLET,
	JUMPER,
	INSECT,
	ORB2,
	MINE,
	SKULL,
	WHITE_BULLET,
	MINE_BULLET,
	CANON,
	TELEPORTER,
	EYEBALL,
	INTRO,
	END,
	END2,
	END3,
	BOSS,
	BOSS_ARM_LEFT,
	BOSS_ARM_RIGHT,
	BOSS_HAND_LEFT,
	BOSS_HAND_RIGHT,
};


extern std::unordered_map<std::string, STATENAME>   STATEString;
extern std::unordered_map<TYPE, bool> Allow;

class CGlobalConfig
{
public:
	CGlobalConfig() {};
	~CGlobalConfig() {};
};
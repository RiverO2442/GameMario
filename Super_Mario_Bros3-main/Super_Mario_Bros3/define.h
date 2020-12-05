﻿#pragma once
//Bell
#define BELL_BBOX_WIDTH		16
#define BELL_BBOX_HEIGHT	16

#define BELL_STATE_IDLE		0
#define BELL_STATE_UP		100


#define BELL_ANI			0	


//Breakable Brick
#define BREAKABLE_BRICK_BBOX_WIDTH		16
#define BREAKABLE_BRICK_BBOX_HEIGHT		16

#define BREAKABLE_BRICK_STATE_NORMAL	0
#define BREAKABLE_BRICK_STATE_BREAK		100
#define BREAKABLE_BRICK_STATE_COIN		200



#define BREAKABLE_BRICK_ANI_NORMAL		0
#define BREAKABLE_BRICK_ANI_COIN		1

//Brick
#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

//Coin
#define COIN_BBOX_WIDTH  10
#define COIN_BBOX_HEIGHT 15


#define COIN_STATE_IDLE		0
#define COIN_STATE_UP		100
#define COIN_STATE_DOWN		200

#define COIN_NORMAL			222
#define COIN_CAN_MOVE		333

//FỉeBall
#define FIREBALL_SPEED 0.15f;

#define FIREBALL_BBOX_WIDTH	7
#define FIREBALL_BBOX_HEIGHT 9

#define	FIREBALL_STATE_DIE	90
#define FIREBALL_STATE_FLYING 100


#define FIREBALL_ANI_FLYING 0

#define FIREBALL_GRAVITY	0.0008f
#define FIREBALL_RESET_TIME 5000

//Flower
#define FLOWER_RED_BBOX_WIDTH  16
#define FLOWER_RED_BBOX_HEIGHT 32

#define FLOWER_GREEN_BBOX_WIDTH  16
#define FLOWER_GREEN_BBOX_HEIGHT 24

#define FLOWER_GREEN_CAN_SHOOT_BBOX_WIDTH  16
#define FLOWER_GREEN_CAN_SHOOT_BBOX_HEIGHT 24

#define FLOWER_RED_ANI_LEFT			0
#define FLOWER_RED_ANI_RIGHT		1
#define FLOWER_RED_ANI_LEFT_IDLE	2
#define FLOWER_RED_ANI_RIGHT_IDLE	3
#define FLOWER_GREEN_ANI			4
#define FLOWER_GREEN_CAN_SHOOT_ANI_LEFT			5
#define FLOWER_GREEN_CAN_SHOOT_ANI_RIGHT		6
#define FLOWER_GREEN_CAN_SHOOT_ANI_LEFT_IDLE	7
#define FLOWER_GREEN_CAN_SHOOT_ANI_RIGHT_IDLE	8
#define FLOWER_RED_ANI_LEFT_UP						9
#define FLOWER_RED_ANI_RIGHT_UP						10
#define FLOWER_RED_ANI_LEFT_IDLE_UP					11
#define FLOWER_RED_ANI_RIGHT_IDLE_UP				12
#define FLOWER_GREEN_CAN_SHOOT_ANI_LEFT_UP			13
#define FLOWER_GREEN_CAN_SHOOT_ANI_RIGHT_UP			14
#define FLOWER_GREEN_CAN_SHOOT_ANI_LEFT_IDLE_UP		15
#define FLOWER_GREEN_CAN_SHOOT_ANI_RIGHT_IDLE_UP	16

#define FLOWER_RED				100
#define FLOWER_GREEN			200
#define FLOWER_GREEN_CAN_SHOOT  300 


#define FLOWER_RED_TOP_LIMIT			72
#define FLOWER_RED_BOT_LIMIT			117
#define RED_TIME_SHOWING_LIMIT			3000

#define FLOWER_GREEN_TOP_LIMIT			96
#define FLOWER_GREEN_BOT_LIMIT			126
#define GREEN_TIME_SHOWING_LIMIT		2000


#define FLOWER_GREEN_CAN_SHOOT_TOP_LIMIT		79
#define FLOWER_GREEN_CAN_SHOOT_BOT_LIMIT		126
#define GREEN_CAN_SHOOT_TIME_SHOWING_LIMIT		3000

//Flower Bullet
#define FLOWER_BULLET_BBOX_WIDTH  8
#define FLOWER_BULLET_BBOX_HEIGHT 9

#define FLOWER_BULLET_X_LIMIT		72


#define FLOWER_BULLET_FLYING_SPEED		0.05f 

#define FLOWER_BULLET_STATE_FLYING		100
#define FLOWER_BULLET_STATE_HIDDEN		200

#define FLOWER_BULLET_ANI			0

//Game
#define DIRECTINPUT_VERSION 0x0800
#define KEYBOARD_BUFFER_SIZE 1024

//GameObject
#define ID_TEX_BBOX -100		// special texture to draw object bounding box

//Goomba
#define GOOMBA_WALKING_SPEED		 0.05f
#define GOOMBA_GRAVITY				 0.002f
#define GOOMBA_DIE_DEFLECT_SPEED	 0.5f
#define GOOMBA_JUMP_SPEED			 0.35f


#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 15
#define GOOMBA_BBOX_HEIGHT_DIE 9

#define GOOMBA_STATE_WALKING	100
#define GOOMBA_STATE_DIE		200
#define GOOMBA_STATE_DIE_2		500
#define GOOMBA_STATE_DISAPPEAR  300
#define GOOMBA_STATE_DIE_BY_KICK		400

#define GOOMBA_NORMAL_ANI_WALKING   0
#define GOOMBA_NORMAL_ANI_DIE		1
#define GOOMBA_RED_FLY_ANI_WALKING	2
#define GOOMBA_RED_FLY_ANI_DIE		3
#define GOOMBA_RED_ANI_NO_WING		4

#define GOOMBA_NORMAL	888
#define GOOMBA_RED_FLY	999

#define GOOMBA_GRAVITY	0.002f

//Koopas
#define KOOPAS_WALKING_SPEED 0.05f;
#define KOOPAS_SPINNING_SPEED 0.3f;

#define KOOPAS_BBOX_WIDTH 15
#define KOOPAS_BBOX_HEIGHT 26
#define KOOPAS_BBOX_HEIGHT_SHELL 16

#define KOOPAS_STATE_WALKING 100
//#define KOOPAS_STATE_WALKING 110
#define KOOPAS_STATE_SHELLING	120
#define KOOPAS_STATE_SPINNING	125
#define KOOPAS_STATE_SHELL_UP	130
#define KOOPAS_STATE_SHELL_DOWN 140
#define KOOPAS_STATE_DIE		200
#define KOOPAS_STATE_DIE_2		400

#define KOOPAS_ANI_WALKING_LEFT		0
#define KOOPAS_ANI_WALKING_RIGHT	1
#define KOOPAS_ANI_FLYING_LEFT		2
#define KOOPAS_ANI_SHELL_DOWN		3
#define KOOPAS_ANI_SHELL_UP			4
#define KOOPAS_RED_ANI_WALKING_LEFT 5
#define KOOPAS_RED_ANI_WALKING_RIGHT 6
#define KOOPAS_RED_ANI_FLYING_LEFT	7
#define KOOPAS_RED_ANI_SHELL_DOWN	8
#define KOOPAS_ANI_SPINNING			9
#define KOOPAS_RED_ANI_SPINNING		10
#define KOOPAS_XANH_ANI_REVIVING	  11
#define KOOPAS_RED_ANI_REVIVING		  12
#define KOOPAS_RED_ANI_SHELL_UP	  13
#define KOOPAS_XANH_ANI_REVIVING_NGUA	14
#define KOOPAS_RED_ANI_REVIVING_NGUA	15
#define KOOPAS_BLACK_UP							16
#define KOOPAS_BLACK_NGUA						17
#define KOOPAS_XANH_ANI_WALKING_RIGHT_FASTER	18
#define KOOPAS_ANI_SPINNING_UP		19
#define KOOPAS_RED_ANI_SPINNING_UP		20

#define KOOPAS_GRAVITY	0.002f

#define KOOPAS_JUMP_SPEED		0.35f
#define KOOPAS_TIME_JUMPING		900

#define KOOPAS_XANH_WALK	111
#define KOOPAS_XANH_FLY		222
#define KOOPAS_RED_WALK		333
#define KOOPAS_RED_FLY		444

//Leaf
#define LEAF_BBOX_WIDTH		16
#define LEAF_BBOX_HEIGHT	14

#define LEAF_STATE_IDLE		0
#define LEAF_STATE_UP		100
#define LEAF_STATE_DOWN		200



#define LEAF_ANI_LEFT		0
#define LEAF_ANI_RIGHT		1

//Mario
#define MARIO_WALKING_SPEED		0.2f 
#define MARIO_WALKING_GIATOC	0.0005f

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.002f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f
#define MARIO_SPEED_DOWN		0.01f 
#define MARIO_MAX_SPEED		0.4f 

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	101
#define MARIO_STATE_JUMP			102
#define MARIO_STATE_DIE				400
#define MARIO_STATE_FLYING_RIGHT	500
#define MARIO_STATE_FLYING_LEFT		600
#define MARIO_STATE_BRAKING_RIGHT	700	
#define MARIO_STATE_BRAKING_LEFT	800
#define MARIO_STATE_HOLDING_RIGHT	900
#define MARIO_STATE_HOLDING_LEFT	1000
#define MARIO_STATE_HOLDING			1100
#define MARIO_STATE_KICK			1200
#define MARIO_STATE_RUNNING_RIGHT	110
#define MARIO_STATE_RUNNING_LEFT	1400
#define MARIO_STATE_SITDOWN			1500
#define MARIO_STATE_LEVEL_CHANGING	1600
#define MARIO_STATE_SPEED_DOWN		1700
#define MARIO_STATE_FALLING_DOWN	1800
#define MARIO_STATE_JUMP_HIGH		1900




#define MARIO_ANI_BIG_IDLE_RIGHT		0
#define MARIO_ANI_BIG_IDLE_LEFT			1
#define MARIO_ANI_BIG_WALKING_RIGHT		2	
#define MARIO_ANI_BIG_WALKING_LEFT		3
#define MARIO_ANI_BIG_JUMPING_RIGHT		4
#define MARIO_ANI_BIG_JUMPING_LEFT		5
#define MARIO_ANI_BIG_FLYING_RIGHT		6	
#define MARIO_ANI_BIG_FLYING_LEFT		7
#define MARIO_ANI_BIG_BRAKING_RIGHT		8	
#define MARIO_ANI_BIG_BRAKING_LEFT		9
#define MARIO_ANI_BIG_HOLDING_RIGHT		10	
#define MARIO_ANI_BIG_HOLDING_LEFT		11
#define MARIO_ANI_BIG_KICKING_RIGHT		12	
#define MARIO_ANI_BIG_KICKING_LEFT		13
#define MARIO_ANI_BIG_RUNNING_RIGHT		14	
#define MARIO_ANI_BIG_RUNNING_LEFT		15
#define MARIO_ANI_BIG_SITDOWN_RIGHT		16	
#define MARIO_ANI_BIG_SITDOWN_LEFT		17
#define MARIO_ANI_BIG_JUMP_FALLING_RIGHT	18
#define MARIO_ANI_BIG_JUMP_FALLING_LEFT		19

#define MARIO_ANI_SMALL_IDLE_RIGHT			20
#define MARIO_ANI_SMALL_IDLE_LEFT			21
#define MARIO_ANI_SMALL_WALKING_RIGHT		22	
#define MARIO_ANI_SMALL_WALKING_LEFT		23
#define MARIO_ANI_SMALL_JUMPING_RIGHT		24
#define MARIO_ANI_SMALL_JUMPING_LEFT		25
#define MARIO_ANI_SMALL_FLYING_RIGHT		26	
#define MARIO_ANI_SMALL_FLYING_LEFT			27
#define MARIO_ANI_SMALL_BRAKING_RIGHT		28	
#define MARIO_ANI_SMALL_BRAKING_LEFT		29
#define MARIO_ANI_SMALL_HOLDING_RIGHT		30	
#define MARIO_ANI_SMALL_HOLDING_LEFT		31
#define MARIO_ANI_SMALL_KICKING_RIGHT		32
#define MARIO_ANI_SMALL_KICKING_LEFT		33
#define MARIO_ANI_SMALL_RUNNING_RIGHT		34	
#define MARIO_ANI_SMALL_RUNNING_LEFT		35


#define MARIO_ANI_TAIL_IDLE_RIGHT		40
#define MARIO_ANI_TAIL_IDLE_LEFT		41
#define MARIO_ANI_TAIL_WALKING_RIGHT	42
#define MARIO_ANI_TAIL_WALKING_LEFT		43
#define MARIO_ANI_TAIL_JUMPING_RIGHT	44
#define MARIO_ANI_TAIL_JUMPING_LEFT		45
#define MARIO_ANI_TAIL_FLYING_RIGHT		46	
#define MARIO_ANI_TAIL_FLYING_LEFT		47
#define MARIO_ANI_TAIL_BRAKING_RIGHT	48	
#define MARIO_ANI_TAIL_BRAKING_LEFT		49
#define MARIO_ANI_TAIL_HOLDING_RIGHT	50	
#define MARIO_ANI_TAIL_HOLDING_LEFT		51
#define MARIO_ANI_TAIL_KICKING_RIGHT	52	
#define MARIO_ANI_TAIL_KICKING_LEFT		53
#define MARIO_ANI_TAIL_RUNNING_RIGHT	54	
#define MARIO_ANI_TAIL_RUNNING_LEFT		55
#define MARIO_ANI_TAIL_SITDOWN_RIGHT	56
#define MARIO_ANI_TAIL_SITDOWN_LEFT		57
#define MARIO_ANI_TAIL_TURNING_LEFT		58
#define MARIO_ANI_TAIL_TURNING_RIGHT	59
#define MARIO_ANI_TAIL_JUMP_FALLING_RIGHT	60
#define MARIO_ANI_TAIL_JUMP_FALLING_LEFT	61

#define MARIO_ANI_FIRE_IDLE_RIGHT		62
#define MARIO_ANI_FIRE_IDLE_LEFT		63
#define MARIO_ANI_FIRE_WALKING_RIGHT	64	
#define MARIO_ANI_FIRE_WALKING_LEFT		65
#define MARIO_ANI_FIRE_JUMPING_RIGHT	66
#define MARIO_ANI_FIRE_JUMPING_LEFT		67
#define MARIO_ANI_FIRE_FLYING_RIGHT		68	
#define MARIO_ANI_FIRE_FLYING_LEFT		69
#define MARIO_ANI_FIRE_BRAKING_RIGHT	70	
#define MARIO_ANI_FIRE_BRAKING_LEFT		71
#define MARIO_ANI_FIRE_HOLDING_RIGHT	72
#define MARIO_ANI_FIRE_HOLDING_LEFT		73
#define MARIO_ANI_FIRE_KICKING_RIGHT	74	
#define MARIO_ANI_FIRE_KICKING_LEFT		75
#define MARIO_ANI_FIRE_RUNNING_RIGHT	76	
#define MARIO_ANI_FIRE_RUNNING_LEFT		77
#define MARIO_ANI_FIRE_SITDOWN_RIGHT	78
#define MARIO_ANI_FIRE_SITDOWN_LEFT		79
#define MARIO_ANI_SHOOTING_RIGHT		80
#define MARIO_ANI_SHOOTING_LEFT			81
#define MARIO_ANI_BIG_MAX_SPEED_RIGHT	82
#define MARIO_ANI_BIG_MAX_SPEED_LEFT	83
#define MARIO_ANI_FIRE_MAX_SPEED_RIGHT	84
#define MARIO_ANI_FIRE_MAX_SPEED_LEFT	85
#define MARIO_ANI_SMALL_MAX_SPEED_RIGHT	86
#define MARIO_ANI_SMALL_MAX_SPEED_LEFT	87
#define MARIO_ANI_TAIL_MAX_SPEED_RIGHT	88
#define MARIO_ANI_TAIL_MAX_SPEED_LEFT	89
#define	MARIO_ANI_FALLING_LEFT			90
#define MARIO_ANI_FALLING_RIGHT			91
#define	MARIO_ANI_FIRE_JUMP_FALL_RIGHT	92
#define MARIO_ANI_FIRE_JUMP_FALL_LEFT	93


#define MARIO_ANI_BIG_IDLE_HOLDING_RIGHT		94	
#define MARIO_ANI_BIG_IDLE_HOLDING_LEFT			95
#define MARIO_ANI_SMALL_IDLE_HOLDING_RIGHT		96	
#define MARIO_ANI_SMALL_IDLE_HOLDING_LEFT		97
#define MARIO_ANI_TAIL_IDLE_HOLDING_RIGHT		98	
#define MARIO_ANI_TAIL_IDLE_HOLDING_LEFT		99
#define MARIO_ANI_FIRE_IDLE_HOLDING_RIGHT		100	
#define MARIO_ANI_FIRE_IDLE_HOLDING_LEFT		101


#define MARIO_GREEN_ANI_BIG_IDLE_RIGHT			102
#define MARIO_GREEN_ANI_BIG_IDLE_LEFT			103
#define MARIO_GREEN_ANI_BIG_WALKING_RIGHT		104
#define MARIO_GREEN_ANI_BIG_WALKING_LEFT		105
#define MARIO_GREEN_ANI_BIG_JUMPING_RIGHT		106
#define MARIO_GREEN_ANI_BIG_JUMPING_LEFT		107
#define MARIO_GREEN_ANI_BIG_HOLDING_IDLE_RIGHT	108
#define MARIO_GREEN_ANI_BIG_HOLDING_IDLE_LEFT	109
#define MARIO_GREEN_ANI_BIG_HOLDING_RIGHT		110
#define MARIO_GREEN_ANI_BIG_HOLDING_LEFT		111
#define MARIO_GREEN_ANI_BIG_KICKING_RIGHT		112
#define MARIO_GREEN_ANI_BIG_KICKING_LEFT		113
#define MARIO_GREEN_ANI_JUMP_FALL_RIGHT			114
#define MARIO_GREEN_ANI_JUMP_FALL_LEFT			115
#define MARIO_GREEN_ANI_BIG_RUNNING_RIGHT		116
#define MARIO_GREEN_ANI_BIG_RUNNING_LEFT		117

#define MARIO_RED_ANI_HITTED					118
#define MARIO_RED_LOOKING_UP					119

#define MARIO_ANI_DIE							120

#define MARIO_TYPE_RED			1
#define MARIO_TYPE_GREEN		2

#define	MARIO_LEVEL_BIG		2
#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_TAIL	3
#define	MARIO_LEVEL_FIRE	4
#define MARIO_BIG_BBOX_WIDTH  15
#define MARIO_BIG_BBOX_HEIGHT 27

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15

#define MARIO_BBOX_HEIGHT_DIFF 12

#define MARIO_UNTOUCHABLE_TIME 1000

#define MARIO_KICKING_TIME 200

#define MARIO_SPINING_TIME 400

#define MARIO_SPEEDUP_TIME 30

#define MARIO_MIN_BRAKING_TIME 400

#define MARIO_MAX_STACK			7

//MushRoom
#define MUSHROOM_BBOX_WIDTH		16
#define MUSHROOM_BBOX_HEIGHT	16

#define MUSHROOM_STATE_IDLE		0
#define MUSHROOM_STATE_UP		100
#define MUSHROOM_STATE_MOVE		200

#define MUSHROOM_GRAVITY		0.002f
#define MUSHROOM_RED			567
#define MUSHROOM_GREEN			678

#define MUSHROOM_ANI_RED		0
#define MUSHROOM_ANI_GREEN		1

//Nocollision Object
#define OBJECT_BBOX_WIDTH  16
#define OBJECT_BBOX_HEIGHT 16

//PIPE
#define RECT_BBOX_WIDTH  16
#define RECT_BBOX_HEIGHT 16

//Question Brick
#define QUESTION_BRICK_BBOX_WIDTH  16
#define QUESTION_BRICK_BBOX_HEIGHT 16



#define QUESTION_BRICK_ANI_ALIVE	0
#define QUESTION_BRICK_ANI_DEAD		1
#define QUESTION_BRICK_ANI_NEW_TYPE	2


#define QUESTION_BRICK_HAVE_LEAF			777
#define QUESTION_BRICK_JUST_HAVE_MUSHROOM	888

//RECT
#define RECT_BBOX_WIDTH  16
#define RECT_BBOX_HEIGHT 16

//PlayScene
#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6

#define OBJECT_TYPE_MARIO				 0
#define OBJECT_TYPE_BRICK				 1
#define OBJECT_TYPE_GOOMBA_NORMAL		 2
#define OBJECT_TYPE_KOOPAS_XANH_WALK	 3
#define OBJECT_TYPE_NO_COLLISION_OBJECTS 4
#define OBJECT_TYPE_RECTANGLE			 5
#define OBJECT_TYPE_PIPE				 6
#define OBJECT_TYPE_KOOPAS_XANH_BAY		 7 
#define OBJECT_TYPE_KOOPAS_RED_WALK		 8
#define OBJECT_TYPE_COIN_NORMAL			 10
#define OBJECT_TYPE_GOOMBA_RED_FLY  11 
#define OBJECT_TYPE_FIRE_BULLET		12
#define OBJECT_TYPE_FLOWER_RED		13
#define OBJECT_TYPE_FLOWER_BULLET	14
#define OBJECT_TYPE_COIN_CAN_MOVE	15
#define OBJECT_TYPE_LEAF			16
#define OBJECT_TYPE_MUSHROOM_RED		17
#define OBJECT_TYPE_QUESTION_BRICK_HAVE_LEAF	18
#define OBJECT_TYPE_MUSHROOM_GREEN		19
#define OBJECT_TYPE_QUESTION_BRICK_JUST_HAVE_MUSHROOM	20
#define OBJECT_TYPE_FLOWER_GREEN				21
#define OBJECT_TYPE_FLOWER_GREEN_CAN_SHOOT		22
#define OBJECT_TYPE_BREAKABLE_BRICK				23
#define OBJECT_TYPE_BELL						24
#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024

//

class define
{
};


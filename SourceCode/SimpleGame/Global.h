#pragma once
/*
열거형 상수, 매직넘버, 매크로 등은 한 곳에서만 정의하고 관리한다.
*/

// Window Option
#define WND_WIDTH 1280
#define WND_HEIGHT 800

// Frame Option
#define MAX_FPS 60 // Frame Per Second (1초당 렌더링횟수 제한)


// Game Elements
#define OBJECT_STATE_GROUND 0
#define OBJECT_STATE_AIR    1
#define HERO_INIT_HP        6
#define BOSS_INIT_HP        100
#define BULLET_DAMAGE       1

// Game Dealay Elements
#define KIND_HERO_DEALAY_HIT                 0
#define KIND_HERO_DEALAY_HIT_COLORCHANGE     1
#define KIND_HERO_DEALAY_SHOOT               2
#define KIND_HERO_DEALAY_DEADSEQUENCE_CHANGE 3
#define KIND_BOSS_DEALAY_PATTERN             0
#define HERO_HIT_DEALAY                  2000 // millisecond
#define HERO_HIT_COLORCHANGE_DEALAY      (1000 / MAX_FPS) * 5 // millisecond
#define HERO_SHOOT_DEALAY                300 // millisecond
#define HERO_DEADSEQUENCE_CHANGE_DEALAY  (1000 / MAX_FPS) * 20 // millisecond


// Object IDs
#define MAX_OBJECTS 1000
#define PRESSURE_PLATE_ID 0 
#define HERO_BODY_ID 1
#define HERO_HEAD_ID 2
#define BOSS_ID      3


// Object Kinds
#define MAX_TEXTURE 13
#define KIND_NULL  -1
#define KIND_PRESSSURE_PLATE 0
#define KIND_HERO_BODY       1
#define KIND_HERO_HEAD       2
#define KIND_HERO_DEADBODY   3
#define KIND_BULLET_NORMAL   4 // Normal Tear
#define KIND_BULLET_BLOOD    5 // Blood Tear
#define KIND_LAND            6
#define KIND_BOSS            7
#define KIND_HERO_HEART	     8 // 8 ~ 11 : Game UI
#define KIND_BOSS_NAME       9
#define KIND_GAME_FAIL       10
#define KIND_GAME_CLEAR      11
#define KIND_RAID_START_INFO 12


// Render Option
#define RENDER_TRANSLATION_SCALE 100.0f
#define MAX_HERO_HEAD_ANIMATION_SEQUENCE_X      8
#define MAX_HERO_HEAD_ANIMATION_SEQUENCE_Y      1
#define MAX_HERO_BODY_ANIMATION_SEQUENCE_X      10
#define MAX_HERO_BODY_ANIMATION_SEQUENCE_Y      3
#define MAX_HERO_HEART_ANIMATION_SEQUENCE_X     4
#define MAX_HERO_HEART_ANIMATION_SEQUENCE_Y     4
#define MAX_HERO_DEAD_ANIMATION_SEQUENCE_X      3
#define MAX_HERO_DEAD_ANIMATION_SEQUENCE_Y      1
#define MAX_BOSS_ANIMATION_SEQUENCE_X           9
#define MAX_BOSS_ANIMATION_SEQUENCE_Y           2
#define MAX_PRESSURE_PLATE_ANIMATION_SEQUENCE_X 2
#define MAX_PRESSURE_PLATE_ANIMATION_SEQUENCE_Y 1
#define HERO_WIDTH             1.0f
#define HERO_HEIGHT            1.0f
#define BOSS_WIDTH             1.5f
#define BOSS_HEIGHT            1.5f
#define BULLET_WIDTH           0.4f
#define BULLET_HEIGHT          0.4f
#define PRESSURE_PLATE_WIDTH   1.0f
#define PRESSURE_PLATE_HEIGHT  1.0f
#define UI_HERO_HEART_WIDTH    0.5f	
#define UI_HERO_HEART_HEIGHT   0.5f
#define UI_HERO_HEART_POS_X    -(WND_WIDTH / 2.0f) + 80.f
#define UI_HERO_HEART_POS_Y    WND_HEIGHT / 2.0f - 90.f
#define UI_BOSS_HP_RECT_POS_X  0.0f
#define UI_BOSS_HP_RECT_POS_Y  WND_HEIGHT / 2.0f - 70.0f
#define UI_BOSS_HP_RECT_WIDTH  WND_WIDTH / 2.0f
#define UI_BOSS_HP_RECT_HEIGHT WND_HEIGHT / 20.0f


// Sound IDs
#define MAX_SOUNDS 12
#define SOUND_INTRO_BGM           0
#define SOUND_BOSSRAID_BGM        1
#define SOUND_HERO_HURT_1         2
#define SOUND_HERO_HURT_2         3
#define SOUND_HERO_HURT_3         4
#define SOUND_HERO_DIES           5
#define SOUND_BOSS_HIGHJUMP       6
#define SOUND_BOSS_LANDING        7
#define SOUND_BOSS_SHOT           8
#define SOUND_TEAR_FIRE           9
#define SOUND_TEAR_BLOCK          10
#define SOUND_PRESSUREPALTE_PRESS 11


// Phisical Option
#define GravityAccelarationFactor 9.80665f
#define HERO_SPEED                1.0f
#define HERO_MAX_SPEED            HERO_SPEED * 5
#define HERO_JUMP_SPEED           HERO_SPEED * 30
#define HERO_MASS                 0.1f
#define HERO_FRICTION_FACTOR      0.5f
#define HERO_BOUNDINGBOX_WIDTH    HERO_WIDTH * 0.75f
#define HERO_BOUNDINGBOX_HEIGHT   HERO_HEIGHT * 0.6f
#define HERO_HEAD_OFFSET_X        0.091f
#define HERO_HEAD_OFFSET_Y        0.411f
#define HERO_FOOTSTEP_DISTANCE    64.0f / RENDER_TRANSLATION_SCALE // 실제 Pixel(17.0f)의 4배(값이 클 수록 Sequence 전환이 느려짐)
#define HERO_FOOTSTEP_DIST_ONE_SEQUENCE (HERO_FOOTSTEP_DISTANCE) / MAX_HERO_BODY_ANIMATION_SEQUENCE_X
#define BOSS_SPEED                2.5f
#define BOSS_JUMP_SPEED           10.0f
#define BOSS_HIGHJUMP_SPEED       35.0f
#define BOSS_MASS                 0.1f
#define BOSS_FRICTION_FACTOR      0.5f
#define BOSS_BOUNDINGBOX_WIDTH    (BOSS_WIDTH * 0.5f) * 0.85f
#define BOSS_BOUNDINGBOX_HEIGHT   (BOSS_HEIGHT * 0.5f) * 0.75f
#define BULLET_SPEED_FOR_HERO     10.0f
#define BULLET_SPEED_FOR_BOSS     2.0f
#define BULLET_MASS               0.01f
#define BULLET_FRICTION_FACTOR    0.01f
#define BULLET_BOUNDINGBOX_WIDTH  BULLET_WIDTH
#define BULLET_BOUNDINGBOX_HEIGHT BULLET_HEIGHT
#define PRESSURE_PLATE_BOUNDINGBOX_WIDTH  PRESSURE_PLATE_WIDTH * 0.2f
#define PRESSURE_PLATE_BOUNDINGBOX_HEIGHT PRESSURE_PLATE_HEIGHT * 0.1f
#define WORLD_BOUNDARY_LEFT    (-WND_WIDTH / 2 + 110.0f) / RENDER_TRANSLATION_SCALE
#define WORLD_BOUNDARY_TOP     (WND_HEIGHT / 2 - 110.0f) / RENDER_TRANSLATION_SCALE
#define WORLD_BOUNDARY_RIGHT   (WND_WIDTH / 2 - 110.0f) / RENDER_TRANSLATION_SCALE
#define WORLD_BOUNDARY_BOTTOM  (-WND_HEIGHT / 2 + 110.0f) / RENDER_TRANSLATION_SCALE


// Player Commands
#define SHOOT_NONE -1
#define SHOOT_LEFT  0
#define SHOOT_RIGHT 1
#define SHOOT_UP    2
#define SHOOT_DOWN  3

// Boss Commands
#define KIND_BOSS_PATTERN     3
#define BOSS_PATTERN_JUMP     0
#define BOSS_PATTERN_HIGHJUMP 1
#define BOSS_PATTERN_SHOOT    2
#define BOSS_SHOOT_PATTERN_NONE -1
#define BOSS_SHOOT_PATTERN_1     0
#define BOSS_SHOOT_PATTERN_2     1
#define BOSS_SHOOT_PATTERN_3     2

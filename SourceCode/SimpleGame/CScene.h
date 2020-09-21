#pragma once
#include "Renderer.h"
#include "Sound.h"
#include "CGameObject.h"
#include "Global.h"
#include <list>


// �������α׷����� ���� Scene
class CScene;
class CPlayScene;




class CScene
{
protected:
	bool         m_KeyState[256] = { false, };
	bool         m_SpecialKeyState[246] = { false, };

	int          m_MouseButton;
	int          m_MouseState;
	int          m_MousePointX;
	int          m_MousePointY;

public:
	virtual bool InitialRenderer(int windowSizeX, int windowSizeY, float TranslationScale = 1.0f) { return false; }
	virtual bool InitialSound() { return false; }
	virtual bool InitialObjects() { return false; }

	virtual void Idle(void) {}

	virtual void MouseInput(int button, int state, int x, int y) {}
	virtual void MouseOperation(float elapsedTime) {}
	virtual void KeyPressed(unsigned char key, int x, int y) {}
	virtual void KeyUp(unsigned char key, int x, int y) {}
	virtual void SpecialKeyPressed(int key, int x, int y) {}
	virtual void SpecialKeyUp(int key, int x, int y) {}
	virtual void KeyOperation(float elapsedTime) {}
	virtual void SpecialKeyOperation(float elapsedTime) {}

	virtual void UpdateScene(float elapsedTime) {}
	virtual void RendrScene() {}
};





// 1pixel : 1cm, ���ӻ󿡼��� ���� �ּҴ����� 1m
/*
m_TranslationScale = 100�̸� ���ӻ󿡼��� 100pixel ��, 1m�� �������̴�.
���⼭ ���ϴ� �������̶� �̵�, ũ��, ȸ�� �� ��ü�� ��� ���庯ȯ�� ����ġ�� �ǹ��Ѵ�.
*/

class GameScene : public CScene
{
private:
	std::list<int> m_KeyStack;
	float          m_TranslationScale = 1.0f;
	Renderer*      m_pRenderer = NULL;
	Sound*         m_pSound = NULL;
	CGameObject*   m_Objects[MAX_OBJECTS];
	int            m_TextureIDs[MAX_TEXTURE];
	int            m_SoundIDs[MAX_SOUNDS];

	bool           m_StartBossRaid = false;
	short          m_BossPattern = BOSS_PATTERN_HIGHJUMP;
	float          m_BossETime = 0.0f;
	bool           m_BossPatternInit = true;

	int  FindEmptyObjectSlot();
	void AddObject(int kind, int State, int HP, int Damage, Point Position, fSIZE Size, fSIZE BoundingBoxSize, float mass, float frictionFactor, Vector Velocity);
	void DeleteObject(int id);
	void DoGarbageCollect();

	void BulletShoot(int BulletKind, int ShootID);
	void ProcessCollision(int i, int j);
	void ProcessCollision();
	void ProcessCollision_BoundaryOut(int i);

	void CreateBoss();
	void BossPattern(float ElapsedTime);
	void BossJump(float ElapsedTime);
	void BossHighJump(float ElapsedTime);
	void BossShoot(float ElapsedTime);
	Vector BossGetTargetDirectirion();
	Point BossGetTargetPosition();

	void RenderUnderUI();
	void RenderUI();
public:
	GameScene();
	~GameScene();

	virtual bool InitialRenderer(int windowSizeX, int windowSizeY, float TranslationScale = 1.0f);
	virtual bool InitialSound();
	virtual bool InitialObjects();

	virtual void Idle(void);

	virtual void MouseInput(int button, int state, int x, int y);
	virtual void MouseOperation(float elapsedTime);
	virtual void KeyPressed(unsigned char key, int x, int y);
	virtual void KeyUp(unsigned char key, int x, int y);
	virtual void SpecialKeyPressed(int key, int x, int y);
	virtual void SpecialKeyUp(int key, int x, int y);
	virtual void KeyOperation(float elapsedTime);
	virtual void SpecialKeyOperation(float elapsedTime);

	virtual void UpdateScene(float elapsedTime);
	virtual void RendrScene();
};
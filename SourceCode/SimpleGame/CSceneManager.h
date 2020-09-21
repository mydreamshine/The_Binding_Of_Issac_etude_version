#pragma once
#include "CScene.h"

class CSceneManager
{
private:
	CScene*      m_pScene = nullptr;
public:
	CSceneManager() = default;
	~CSceneManager();

	void Idle(void);

	void MouseInput(int button, int state, int x, int y);
	void MouseOperation(float elapsedTime);
	void KeyPressed(unsigned char key, int x, int y);
	void KeyUp(unsigned char key, int x, int y);
	void SpecialKeyPressed(int key, int x, int y);
	void SpecialKeyUp(int key, int x, int y);
	void KeyOperation(float elapsedTime);
	void SpecialKeyOperation(float elapsedTime);

	void SetScene(CScene* newScene);
	void UpdateScene(float elapsedTime);
	void RendrScene();
};


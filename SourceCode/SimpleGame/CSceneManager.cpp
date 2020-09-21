#include "stdafx.h"
#include "CSceneManager.h"

CSceneManager::~CSceneManager()
{
	delete m_pScene;
}

void CSceneManager::Idle(void)
{
	if (m_pScene) m_pScene->Idle();
}

void CSceneManager::MouseInput(int button, int state, int x, int y)
{
	if (m_pScene) m_pScene->MouseInput(button, state, x, y);
}

void CSceneManager::MouseOperation(float elapsedTime)
{
	if (m_pScene) m_pScene->MouseOperation(elapsedTime);
}

void CSceneManager::KeyPressed(unsigned char key, int x, int y)
{
	if (m_pScene) m_pScene->KeyPressed(key, x, y);
}

void CSceneManager::KeyUp(unsigned char key, int x, int y)
{
	if (m_pScene) m_pScene->KeyUp(key, x, y);
}

void CSceneManager::SpecialKeyPressed(int key, int x, int y)
{
	if (m_pScene) m_pScene->SpecialKeyPressed(key, x, y);
}

void CSceneManager::SpecialKeyUp(int key, int x, int y)
{
	if (m_pScene) m_pScene->SpecialKeyUp(key, x, y);
}

void CSceneManager::KeyOperation(float elapsedTime)
{
	if (m_pScene) m_pScene->KeyOperation(elapsedTime);
}

void CSceneManager::SpecialKeyOperation(float elapsedTime)
{
	if (m_pScene) m_pScene->SpecialKeyOperation(elapsedTime);
}

/*
업데이트 함수는 다음과 같이 크게 3가지로 분류될 수 있다.
1. 매 프레임마다 호출되는 업데이트 함수
2. 특정주기마다 호출되는 업데이트 함수
3. 이벤트에 따라 호출되는 업데이트 함수

업데이트 함수는 컴퓨터 성능에 따라 호출되는 횟수가 달라질 수 있다.
CPU와 GPU의 싱크를 맞추질 않았기 때문. -> 싱크는 SimpleGame.cpp의 RenderScene함수에서 timeGetTime()함수로 맞춰주고 있다.
*/

void CSceneManager::SetScene(CScene* newScene)
{
	if (m_pScene) { delete m_pScene; m_pScene = nullptr; }
	m_pScene = newScene;
}

void CSceneManager::UpdateScene(float elapsedTime)
{
	if (m_pScene) m_pScene->UpdateScene(elapsedTime);
}

void CSceneManager::RendrScene()
{
	if (m_pScene) m_pScene->RendrScene();
}

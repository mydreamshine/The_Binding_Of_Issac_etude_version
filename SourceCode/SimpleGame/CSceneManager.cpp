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
������Ʈ �Լ��� ������ ���� ũ�� 3������ �з��� �� �ִ�.
1. �� �����Ӹ��� ȣ��Ǵ� ������Ʈ �Լ�
2. Ư���ֱ⸶�� ȣ��Ǵ� ������Ʈ �Լ�
3. �̺�Ʈ�� ���� ȣ��Ǵ� ������Ʈ �Լ�

������Ʈ �Լ��� ��ǻ�� ���ɿ� ���� ȣ��Ǵ� Ƚ���� �޶��� �� �ִ�.
CPU�� GPU�� ��ũ�� ������ �ʾұ� ����. -> ��ũ�� SimpleGame.cpp�� RenderScene�Լ����� timeGetTime()�Լ��� �����ְ� �ִ�.
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

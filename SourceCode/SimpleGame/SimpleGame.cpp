
#include "stdafx.h"
#include <iostream>
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"
#include <time.h>
#include <timeapi.h>

#include "CSceneManager.h"

CSceneManager* g_pSceneManager = nullptr;

void RenderScene(void)
{
	static char FrameAndCommunicationRate[50] = "The Binding of Isaac - Client (";
	static DWORD g_PrevRenderTime = 0;
	static DWORD cur_Time = 0;
	static float eTime = 0.0f;

	if (g_PrevRenderTime == 0)
		g_PrevRenderTime = timeGetTime();
	cur_Time = timeGetTime();
	eTime = (float)(cur_Time - g_PrevRenderTime) / 1000.0f;
	//일정 프레임이 나오도록 변경
	if (eTime > 1.0f / MAX_FPS) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.3f, 0.3f, 1.0f);

		g_PrevRenderTime = cur_Time;
		_itoa_s((int)(1.0f / eTime), FrameAndCommunicationRate + 31, 19, 10);
		strcat_s(FrameAndCommunicationRate + 31, 19, " FPS)");

		g_pSceneManager->UpdateScene(eTime);
		g_pSceneManager->MouseOperation(eTime);
		g_pSceneManager->KeyOperation(eTime);
		g_pSceneManager->SpecialKeyOperation(eTime);

		glutSetWindowTitle(FrameAndCommunicationRate);

		g_pSceneManager->RendrScene();
		glutSwapBuffers();
	}
}

/*
void idle() : glut 업데이트 함수
*/
void Idle(void)
{
	g_pSceneManager->Idle();
	RenderScene();
}

void MouseInput(int button, int state, int x, int y)
{
	g_pSceneManager->MouseInput(button, state, x, y);
}

void KeyPressed(unsigned char key, int x, int y)
{
	g_pSceneManager->KeyPressed(key, x, y);
}

void KeyUp(unsigned char key, int x, int y)
{
	g_pSceneManager->KeyUp(key, x, y);
}

void SpecialKeyPressed(int key, int x, int y)
{
	g_pSceneManager->SpecialKeyPressed(key, x, y);
}

void SpecialKeyUp(int key, int x, int y)
{
	g_pSceneManager->SpecialKeyUp(key, x, y);
}


int main(int argc, char **argv)
{
	// Initialize GL things
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WND_WIDTH, WND_HEIGHT);
	glutCreateWindow("The Binding of Isaac - Client (");
	srand(static_cast <unsigned> (time(0)));

	glewInit();
	if (glewIsSupported("GL_VERSION_3_0"))
	{
		std::cout << " GLEW Version is 3.0\n ";
	}
	else
	{
		std::cout << "GLEW 3.0 not supported\n ";
	}

	// Initialize Renderer (WndWidth, WndHeight, TranslationScale)
	CScene* pScene = new GameScene();
	if (!pScene->InitialRenderer(WND_WIDTH, WND_HEIGHT, RENDER_TRANSLATION_SCALE))
	{
		std::cout << "Renderer could not be initialized.. \n";
		delete pScene;
		return 0;
	}
	// Initialize Sound
	if (!pScene->InitialSound())
	{
		std::cout << "Sound could not be initialized.. \n";
		delete pScene;
		return 0;
	}
	// Initialize Object
	if (!pScene->InitialObjects())
	{
		std::cout << "Objects could not be initialized.. \n";
		delete pScene;
		return 0;
	}
	g_pSceneManager = new CSceneManager();
	g_pSceneManager->SetScene(pScene);

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);

	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutKeyboardFunc(KeyPressed);
	glutKeyboardUpFunc(KeyUp);
	glutSpecialFunc(SpecialKeyPressed);
	glutSpecialUpFunc(SpecialKeyUp);
	glutMouseFunc(MouseInput);

	glutMainLoop();

	delete g_pSceneManager;
    return 0;
}


#include "stdafx.h"
#include "Renderer.h"
#include "LoadPng.h"
#include <Windows.h>
#include <cstdlib>

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}


Renderer::~Renderer()
{
}

void Renderer::GetGLPosition(float x, float y, float *newX, float *newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}

void Renderer::GetGLShadow(float x,float *newV)
{
	*newV = x / m_WindowSizeY;
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	m_SolidRectShader = CompileShaders("./Resource/Shaders/SolidRect.vs", "./Resource/Shaders/SolidRect.fs");
	m_TextureRectShader = CompileShaders("./Resource/Shaders/TextureRect.vs", "./Resource/Shaders/TextureRect.fs");
	m_TextureRectSeqShader = CompileShaders("./Resource/Shaders/TextureRectSeq.vs", "./Resource/Shaders/TextureRectSeq.fs");

	//Load shadow texture
	m_TexShadow = CreatePngTexture("./Resource/Graphic/shadow.png");

	//Create VBOs
	CreateVertexBufferObjects();

	if (m_SolidRectShader > 0 && m_VBORect > 0)
	{
		m_Initialized = true;
	}
}

bool Renderer::IsInitialized()
{
	return m_Initialized;
}

void Renderer::CreateVertexBufferObjects()
{
	float rect[]
		=
	{
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, 
		-1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 
		1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, //Triangle1
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f,  
		1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 
		1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, //Triangle2
	};

	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	float texRect[]
		=
	{
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, 0.f, 0.f,
		-1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 0.f, 1.f,
		1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f, 1.f,//Triangle1
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, 0.f, 0.f,
		1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f, 1.f,
		1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, 1.f, 0.f//Triangle2
	};

	glGenBuffers(1, &m_VBOTexRect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexRect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texRect), texRect, GL_STATIC_DRAW);
}

GLuint Renderer::CreatePngTexture(char * filePath)
{
	GLuint temp;
	glGenTextures(1, &temp);

	//Load Pngs
	// Load file and decode image.
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filePath);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	return temp;
}

void Renderer::DeleteTexture(GLuint texID)
{
	glDeleteTextures(1, &texID);
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string *target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.\n";

	return ShaderProgram;
}

void Renderer::DrawSolidRect(float x, float y, float z, float sizeX, float sizeY, float r, float g, float b, float a)
{
	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	//Program select
	glUseProgram(m_SolidRectShader);

	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Trans"), newX, newY, sizeX, sizeY);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Color"), r, g, b, a);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::DrawTextureRect(float x, float y, float z, float sizeX, float sizeY, float r, float g, float b, float a, GLuint texID)
{
	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Program select
	glUseProgram(m_TextureRectShader);

	glUniform4f(glGetUniformLocation(m_TextureRectShader, "u_Trans"), newX, newY, sizeX, sizeY);
	glUniform4f(glGetUniformLocation(m_TextureRectShader, "u_Color"), r, g, b, a);
	int texUniform = glGetUniformLocation(m_TextureRectShader, "u_Texture");
	glUniform1i(texUniform, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);

	int attribPosition = glGetAttribLocation(m_TextureRectShader, "a_Position");
	int attribTexture = glGetAttribLocation(m_TextureRectShader, "a_TexPos");

	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(attribTexture);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexRect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(attribTexture, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(3 * sizeof(float)));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribTexture);

	glDisable(GL_BLEND);
}

void Renderer::DrawTextureRectHeight(float x, float y, float z, float sizeX, float sizeY, float r, float g, float b, float a, GLuint texID, float height)
{
	if (height < 0.f)
		return;

	float newX, newY, shadowY, baseY;

	GetGLPosition(x, y, &newX, &newY);
	GetGLShadow(height, &shadowY);
	GetGLShadow(sizeY/2.f, &baseY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Program select
	glUseProgram(m_TextureRectShader);

	glUniform4f(glGetUniformLocation(m_TextureRectShader, "u_Trans"), newX, newY, sizeX, sizeY);
	glUniform4f(glGetUniformLocation(m_TextureRectShader, "u_Color"), 1, 1, 1, a);
	int texUniform = glGetUniformLocation(m_TextureRectShader, "u_Texture");
	glUniform1i(texUniform, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TexShadow);

	int attribPosition = glGetAttribLocation(m_TextureRectShader, "a_Position");
	int attribTexture = glGetAttribLocation(m_TextureRectShader, "a_TexPos");

	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(attribTexture);

	//Render shadow first
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexRect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(attribTexture, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(3 * sizeof(float)));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Render Object
	glUniform4f(glGetUniformLocation(m_TextureRectShader, "u_Trans"), newX, newY + shadowY + baseY, sizeX, sizeY);
	glUniform4f(glGetUniformLocation(m_TextureRectShader, "u_Color"), r, g, b, a);

	glBindTexture(GL_TEXTURE_2D, texID);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribTexture);

	glDisable(GL_BLEND);
}

void Renderer::DrawTextureRectSeqXY(float x, float y, float z, float sx, float sy, float r, float g, float b, float a, GLuint texID, int currSeqX, int currSeqY, int totalSeqX, int totalSeqY)
{
	GLuint tID = texID;

	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	GLuint shader = m_TextureRectSeqShader;

	//Program select
	glUseProgram(shader);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	GLuint u_Trans = glGetUniformLocation(shader, "u_Trans");
	GLuint u_Color = glGetUniformLocation(shader, "u_Color");
	GLuint u_TotalSeqX = glGetUniformLocation(shader, "u_TotalSeqX");
	GLuint u_TotalSeqY = glGetUniformLocation(shader, "u_TotalSeqY");
	GLuint u_CurrSeqX = glGetUniformLocation(shader, "u_CurrSeqX");
	GLuint u_CurrSeqY = glGetUniformLocation(shader, "u_CurrSeqY");
	glUniform1f(glGetUniformLocation(shader, "u_Depth"), 0.5f);

	glUniform4f(u_Trans, newX, newY, sx, sy);
	glUniform4f(u_Color, r, g, b, a);
	glUniform1f(u_TotalSeqX, (float)totalSeqX);
	glUniform1f(u_CurrSeqX, (float)currSeqX);
	glUniform1f(u_TotalSeqY, (float)totalSeqY);
	glUniform1f(u_CurrSeqY, (float)currSeqY);

	GLuint attribPosition = glGetAttribLocation(shader, "a_Position");
	GLuint attribTexPosition = glGetAttribLocation(shader, "a_TexPosition");
	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(attribTexPosition);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexRect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(attribTexPosition, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tID);
	GLuint u_Texture = glGetUniformLocation(shader, "u_Texture");
	glUniform1i(u_Texture, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribTexPosition);
}

void Renderer::DrawTextureRectHeightSeqXY(float x, float y, float z, float sx, float sy, float r, float g, float b, float a, GLuint texID, int currSeqX, int currSeqY, int totalSeqX, int totalSeqY)
{
	if (z < 0.f)
		return;

	GLuint tID = texID;

	float newX, newY, shadowY, baseY;

	GetGLPosition(x, y, &newX, &newY);
	GetGLShadow(z, &shadowY);
	GetGLShadow(sy / 20.f, &baseY);

	GLuint shaderSeq = m_TextureRectSeqShader;
	GLuint shaderHeight = m_TextureRectShader;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(shaderHeight);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glUniform4f(glGetUniformLocation(shaderHeight, "u_Trans"), newX + 0.02f, newY, sx, sy);
	glUniform4f(glGetUniformLocation(shaderHeight, "u_Color"), 1, 1, 1, a);
	int texUniform = glGetUniformLocation(shaderHeight, "u_Texture");
	glUniform1i(texUniform, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TexShadow);

	GLuint attribPosition1 = glGetAttribLocation(shaderHeight, "a_Position");
	GLuint attribTexPosition1 = glGetAttribLocation(shaderHeight, "a_TexPos");
	glEnableVertexAttribArray(attribPosition1);
	glEnableVertexAttribArray(attribTexPosition1);

	// Render shadow first
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexRect);
	glVertexAttribPointer(attribPosition1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(attribTexPosition1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition1);
	glDisableVertexAttribArray(attribTexPosition1);

	//Render Object
	glUseProgram(shaderSeq);

	GLuint u_Trans = glGetUniformLocation(shaderSeq, "u_Trans");
	GLuint u_Color = glGetUniformLocation(shaderSeq, "u_Color");
	GLuint u_TotalSeqX = glGetUniformLocation(shaderSeq, "u_TotalSeqX");
	GLuint u_TotalSeqY = glGetUniformLocation(shaderSeq, "u_TotalSeqY");
	GLuint u_CurrSeqX = glGetUniformLocation(shaderSeq, "u_CurrSeqX");
	GLuint u_CurrSeqY = glGetUniformLocation(shaderSeq, "u_CurrSeqY");
	glUniform1f(glGetUniformLocation(shaderSeq, "u_Depth"), 0.5f);

	glUniform4f(u_Trans, newX, newY + shadowY + baseY, sx, sy);
	glUniform4f(u_Color, r, g, b, a);
	glUniform1f(u_TotalSeqX, (float)totalSeqX);
	glUniform1f(u_CurrSeqX, (float)currSeqX);
	glUniform1f(u_TotalSeqY, (float)totalSeqY);
	glUniform1f(u_CurrSeqY, (float)currSeqY);

	GLuint attribPosition = glGetAttribLocation(shaderSeq, "a_Position");
	GLuint attribTexPosition = glGetAttribLocation(shaderSeq, "a_TexPosition");
	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(attribTexPosition);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexRect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(attribTexPosition, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tID);
	GLuint u_Texture = glGetUniformLocation(shaderSeq, "u_Texture");
	glUniform1i(u_Texture, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribTexPosition);
}

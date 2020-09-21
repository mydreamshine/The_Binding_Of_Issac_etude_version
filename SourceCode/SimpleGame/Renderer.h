#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Dependencies\glew.h"

class Renderer
{
public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	bool IsInitialized();
	void DrawSolidRect(float x, float y, float z, float sizeX, float sizeY, float r, float g, float b, float a);
	void DrawTextureRect(float x, float y, float z, float sizeX, float sizeY, float r, float g, float b, float a, GLuint texID);
	void DrawTextureRectHeight(float x, float y, float z, float sizeX, float sizeY, float r, float g, float b, float a, GLuint texID, float height);
	void DrawTextureRectSeqXY(float x, float y, float z, float sizeX, float sizeY, float r, float g, float b, float a, GLuint texID, int currSeqX, int currSeqY, int totalSeqX, int totalSeqY);
	void DrawTextureRectHeightSeqXY(float x, float y, float z, float sx, float sy, float r, float g, float b, float a, GLuint texID, int currSeqX, int currSeqY, int totalSeqX, int totalSeqY);
	GLuint CreatePngTexture(char * filePath);
	void DeleteTexture(GLuint texID);

private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects();
	void GetGLPosition(float x, float y, float *newX, float *newY);
	void GetGLShadow(float x, float *newV);

	bool m_Initialized = false;
	
	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	GLuint m_TexShadow = 0;

	GLuint m_VBORect = 0;
	GLuint m_SolidRectShader = 0;

	GLuint m_VBOTexRect = 0;
	GLuint m_TextureRectShader = 0;

	GLuint m_TextureRectSeqShader = 0;
};


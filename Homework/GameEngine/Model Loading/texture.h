#pragma once
#include <glew.h>
#include <glfw3.h>

enum {
	RIGHT,
	LEFT,
	TOP,
	BOTTOM,
	FRONT,
	BACK
};

GLuint loadBMP(const char* imagepath );

GLuint loadSkybox(const char* imagePaths[]);

int loadSkyboxSide(const char* imagepath, int side);

#pragma once
#include "mesh.h"

class Skybox {

public:

	//Vertices and indices are the same for all skyboxes
	const std::vector<Vertex> vertices = {
		Vertex(-1.0f, -1.0f,  1.0f),
		Vertex( 1.0f, -1.0f,  1.0f),
		Vertex( 1.0f, -1.0f, -1.0f),
		Vertex(-1.0f, -1.0f, -1.0f),
		Vertex(-1.0f,  1.0f,  1.0f),
		Vertex( 1.0f,  1.0f,  1.0f),
		Vertex( 1.0f,  1.0f, -1.0f),
		Vertex(-1.0f,  1.0f, -1.0f)
	};

	const std::vector<int> indices = {
		1, 2, 6,
		6, 5, 1,
		0, 4, 7,
		7, 3, 0,
		4, 5, 6,
		6, 7, 4,
		0, 3, 2,
		2, 1, 0,
		0, 1, 5,
		5, 4, 0,
		3, 7, 6,
		6, 2, 3
	};

	//std::vector <GLuint> textures;

	unsigned int vao, vbo, ibo;
	GLuint textureID;

	Skybox();
	Skybox(GLuint textureID);

	void setup();
	void draw();

};
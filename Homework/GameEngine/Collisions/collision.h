#pragma once
#include "../Statistics/counter.h"
#include "../Model Loading/mesh.h"
#include <glm.hpp>
#include <glfw3.h>
#include <glew.h>

class BoundingBox
{
public:
	BoundingBox() {
		this->center = glm::vec3(0.0f);
		this->halfWidth = 0.5f;
		this->halfHeight = 0.5f;
		this->halfDepth = 0.5f;
		initBBoxes(); 
	};
	glm::vec3 center;
	float halfWidth, halfHeight, halfDepth;

	// Cube 1x1x1, centered on origin
	static constexpr  float BoundingBox::vertices[24] = {
		// front
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};

	static constexpr  float BoundingBox::elements[36] = {
		0, 1, 2,
		2, 3, 0,
		1, 5, 6,
		6, 2, 1,
		7, 6, 5,
		5, 4, 7,
		4, 0, 3,
		3, 7, 4,
		4, 5, 1,
		1, 0, 4,
		3, 2, 6,
		6, 7, 3
	};

	GLuint vao, vbo, ibo;

	void draw();

	static bool checkCollision(const BoundingBox& a, const BoundingBox& b);

	static bool checkCollision(const glm::vec3& a, const BoundingBox& b);

	BoundingBox(const Mesh& mesh);
private:
	void initBBoxes();

};


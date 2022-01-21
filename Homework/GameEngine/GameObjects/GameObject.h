#pragma once


#include "../Model Loading/mesh.h"
#include "../Collisions/collision.h"
#include <glfw3.h>

class GameObject {

public:
	Mesh* mesh;
	BoundingBox boundingbox;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	GameObject(Mesh* mesh, BoundingBox bbox);

	GameObject(Mesh* mesh);

	void draw(Shader shader);
	void setScale(glm::vec3 scale);
	void setScaleNoUpdate(glm::vec3 scale);
	void setPosition(glm::vec3 position);
	void setRotation(glm::vec3 rotation);
private:
	void updateBBox();

};
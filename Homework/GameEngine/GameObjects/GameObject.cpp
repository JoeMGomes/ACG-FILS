#include "GameObject.h"
#include <gtx/transform.hpp>

GameObject::GameObject(Mesh* mesh, BoundingBox bbox)
{
	this->mesh = mesh;
	this->boundingbox = bbox;
	this->position = glm::vec3(0.0f);
	this->rotation = glm::vec3(0.0f);
	this->scale= glm::vec3(1.0f);

}

GameObject::GameObject(Mesh* mesh) {

    this->mesh = mesh;
    this->boundingbox =  BoundingBox(*mesh);
    this->position = glm::vec3(0.0f);
    this->rotation = glm::vec3(0.0f);
    this->scale = glm::vec3(1.0f);
}

void GameObject::draw(Shader shader)
{
	mesh->draw(shader);
}

void GameObject::setScale(glm::vec3 scale) {
	
	this->scale = scale;
	
	this->boundingbox = BoundingBox(*mesh);	
	updateBBox();
}

void GameObject::setScaleNoUpdate(glm::vec3 scale) {
	this->scale = scale;
}

void GameObject::setPosition(glm::vec3 position) {
	this->position = position;
	updateBBox();
}

//DOES NOT UPDATE THE BOUNDING BOX
void GameObject::setRotation(glm::vec3 rotation)
{
	this->rotation = rotation;
}


void GameObject::updateBBox()
{
	this->boundingbox.center = this->position;
	this->boundingbox.halfWidth *= scale.x;
	this->boundingbox.halfHeight *= scale.y;
	this->boundingbox.halfDepth *= scale.z;

}

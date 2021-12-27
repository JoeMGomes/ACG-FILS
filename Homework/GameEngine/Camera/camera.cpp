#include "camera.h"

Camera::Camera(glm::vec3 cameraPosition)
{
	this->cameraPosition = cameraPosition;
	this->cameraViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->cameraRight = glm::cross(cameraViewDirection, cameraUp);
	this->rotationOx = 0.0f;
	this->rotationOy = -90.0f;
}

Camera::Camera()
{
	this ->cameraPosition = glm::vec3(0.0f, 0.0f, 100.0f);
	this ->cameraViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	this ->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->cameraRight = glm::cross(cameraViewDirection, cameraUp);
	this->rotationOx = 0.0f;
	this->rotationOy = -90.0f;
}

Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraViewDirection, glm::vec3 cameraUp)
{
	this->cameraPosition = cameraPosition;
	this->cameraViewDirection = cameraViewDirection;
	this->cameraUp = cameraUp;
	this->cameraRight = glm::cross(cameraViewDirection, cameraUp);
}

Camera::~Camera()
{
}

void Camera::keyboardMoveFront(float cameraSpeed)
{
	cameraPosition += cameraViewDirection * cameraSpeed;
}

void Camera::keyboardMoveBack(float cameraSpeed)
{
	cameraPosition -= cameraViewDirection * cameraSpeed;
}

void Camera::keyboardMoveLeft(float cameraSpeed)
{
	cameraPosition -= glm::normalize(glm::cross(cameraViewDirection, cameraUp)) * cameraSpeed;
}

void Camera::keyboardMoveRight(float cameraSpeed)
{
	cameraPosition += glm::normalize(glm::cross(cameraViewDirection, cameraUp)) * cameraSpeed;
}

void Camera::keyboardMoveUp(float cameraSpeed)
{
	cameraPosition += cameraUp * cameraSpeed;
}

void Camera::keyboardMoveDown(float cameraSpeed)
{
	cameraPosition -= cameraUp * cameraSpeed;
}

void Camera::globalMoveDown(float cameraSpeed)
{
	cameraPosition -= glm::vec3(0, 1, 0) * cameraSpeed;

}

void Camera::FPSForward(float cameraSpeed){

	cameraPosition += glm::normalize(glm::vec3(cameraViewDirection.x, 0,cameraViewDirection.z)) * cameraSpeed;
}

void Camera::FPSRight(float cameraSpeed) {

	cameraPosition += glm::normalize(glm::cross(cameraViewDirection, cameraUp)) * cameraSpeed;
}

void Camera::rotateOx(float angle)
{	
	cameraViewDirection = glm::normalize(glm::vec3((glm::rotate(glm::mat4(1.0f), angle, cameraRight) * glm::vec4(cameraViewDirection, 1))));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraViewDirection));
	cameraRight = glm::cross(cameraViewDirection, cameraUp);
}

void Camera::rotateOy (float angle)
{
	cameraViewDirection = glm::normalize(glm::vec3((glm::rotate(glm::mat4(1.0f), angle, cameraUp) * glm::vec4(cameraViewDirection, 1))));
	cameraRight = glm::normalize(glm::vec3((glm::rotate(glm::mat4(1.0f), angle, cameraUp) * glm::vec4(cameraRight, 1))));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraViewDirection));
}

void Camera::rotate() {

	cameraViewDirection.x = cos(glm::radians(rotationOy)) * cos(glm::radians(rotationOx));
	cameraViewDirection.y = sin(glm::radians(rotationOx));
	cameraViewDirection.z = sin(glm::radians(rotationOy)) * cos(glm::radians(rotationOx));
	cameraViewDirection = glm::normalize(cameraViewDirection);

	cameraRight = glm::normalize(glm::cross(cameraViewDirection, glm::vec3(0,1,0)));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraViewDirection));
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(cameraPosition, cameraPosition + cameraViewDirection, cameraUp);
}

glm::vec3 Camera::getCameraPosition()
{
	return cameraPosition;
}

glm::vec3 Camera::getCameraViewDirection()
{
	return cameraViewDirection;
}

glm::vec3 Camera::getCameraUp()
{
	return cameraUp;
}



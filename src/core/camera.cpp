#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Camera::Camera() :
	position(0.0f, 0.0f, 7.5f),
	oldMousePosition(0.0f),
	viewDirection(0.0f, 0.0f, -1.0f),
	cameraRight(1.0f, 0.0f, 0.0f),
	cameraUp(0.0f, 1.0f, 0.0f),
	moving({ false, false, false, false, false, false }),
	isAccelerating(false),
	fov(45.0f),
	SENSITIVITY(0.00075f),
	MOVEMENT_SPEED(3.0f),
	ACCELERATION_SPEED(3.0f)

{
}

void Camera::mouseUpdate(glm::vec2 newMousePos) {
	glm::vec2 mouseDelta = newMousePos - oldMousePosition;
	oldMousePosition = newMousePos;
	if (glm::length(mouseDelta) > 500.0f) return; // avoid jump on first mouse enter

	cameraRight = glm::normalize(glm::cross(viewDirection, cameraUp));

	glm::mat4 rotator = glm::rotate(glm::mat4(1.0), -mouseDelta.y * SENSITIVITY, cameraRight) * glm::rotate(glm::mat4(1.0), -mouseDelta.x * SENSITIVITY, cameraUp);

	viewDirection = rotator * glm::vec4(viewDirection, 0.0f);
}

glm::mat4 Camera::getWorldToViewMatrix() const {
	return glm::lookAt(position, position + viewDirection, cameraUp);
}

void Camera::move(float delta) {
	glm::vec3 direction = glm::vec3(0.0f);
	direction += viewDirection * (float)(moving[FORWARD] - moving[BACKWARD]);
	direction += cameraRight * (float)(moving[RIGHT] - moving[LEFT]);
	direction += cameraUp * (float)(moving[UP] - moving[DOWN]);

	if (glm::length(direction) > 0.0f) { // to prevent normalizing a zero-vector
		position += glm::normalize(direction) * (MOVEMENT_SPEED + ACCELERATION_SPEED * (float)isAccelerating) * delta;
	}
}

void Camera::setMoving(uint8_t dir, bool state) {
	moving[dir] = state;
}

void Camera::setAcceleration(bool state) {
	isAccelerating = state;
}

glm::vec3 Camera::getPosition() const {
	return position;
}

float Camera::getFov() const {
	return fov;
}

void Camera::zoom(float yoffset) {
	fov -= (float)yoffset;
	if (fov < 1.0f) {
		fov = 1.0f;
	}
	if (fov > 45.0f) {
		fov = 45.0f;
	}
}
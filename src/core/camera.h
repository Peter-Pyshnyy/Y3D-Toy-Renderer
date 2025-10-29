#pragma once
#include <glm/glm.hpp>
#include <array>

class Camera
{
	glm::vec3 position;
	glm::vec3 viewDirection;
	glm::vec3 cameraRight;
	glm::vec3 cameraUp;
	glm::vec2 oldMousePosition;
	std::array<bool, 6> moving; //directional movement-flags
	bool isAccelerating;
	float fov;
	const float SENSITIVITY;
	const float MOVEMENT_SPEED;
	const float ACCELERATION_SPEED;
public:
	Camera();
	void mouseUpdate(glm::vec2 newMousePos);
	void move(float delta);
	void setMoving(uint8_t dir, bool state);
	void setAcceleration(bool state);
	void zoom(float yoffset);
	glm::mat4 getWorldToViewMatrix() const;
	glm::vec3 getPosition() const;
	float getFov() const;
	enum Direction { FORWARD, BACKWARD, RIGHT, LEFT, UP, DOWN };
};


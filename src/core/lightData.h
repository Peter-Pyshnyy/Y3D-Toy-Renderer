#pragma once
#include <glm/glm.hpp>

constexpr float DEFAULT_AMBIENT = 0.1f;
constexpr float DEFAULT_DIFFUSE = 0.8f;
constexpr float DEFAULT_SPECULAR = 1.0f;

struct DirectionalLight {
	glm::vec3 ambient{ DEFAULT_AMBIENT };
	glm::vec3 diffuse{ DEFAULT_DIFFUSE };
	glm::vec3 specular{ DEFAULT_SPECULAR };
	glm::vec3 direction;
};

struct PointLight {
	glm::vec3 ambient{ DEFAULT_AMBIENT };
	glm::vec3 diffuse{ DEFAULT_DIFFUSE };
	glm::vec3 specular{ DEFAULT_SPECULAR };
	glm::vec3 position;
	float constant{ 1.0f };
	float linear{ 0.09f };
	float quadratic{ 0.032f };
};

struct Spotlight {
	glm::vec3 ambient{ DEFAULT_AMBIENT };
	glm::vec3 diffuse{ DEFAULT_DIFFUSE };
	glm::vec3 specular{ DEFAULT_SPECULAR };
	glm::vec3 position;
	glm::vec3 direction;
	float cutOff;      // in degrees
	float outerCutOff; // in degrees
};
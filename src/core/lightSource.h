#pragma once
#include <glm/glm.hpp>

constexpr int MAX_POINTLIGHTS = 16;
constexpr int MAX_SPOTLIGHTS = 16;

enum LightType {
	DIRECTIONAL,
	POINT,
	SPOT
};

class LightSource {
public:
	LightType type;
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

	glm::vec3 direction;
	float cutOff;
	float outerCutOff;

	LightSource(
		glm::vec3 position = glm::vec3(0.0f),
		glm::vec3 ambient = glm::vec3(0.1f),
		glm::vec3 diffuse = glm::vec3(0.8f),
		glm::vec3 specular = glm::vec3(1.0f)
	);

	void constructAsDirectionalLight(glm::vec3 direction);
	void constructAsPointLight(float constant, float linear, float quadratic);
	void constructAsSpotlight(glm::vec3 direction, float cutOff, float outerCutOff);
	static bool incrementLightCount(LightType type);
	static glm::uvec3 getLightCount();

protected:
	static glm::uvec3 lightCount;
};
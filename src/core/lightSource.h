#pragma once
#include <glm/glm.hpp>

constexpr int MAX_POINTLIGHTS = 16;
constexpr int MAX_SPOTLIGHTS = 16;
constexpr float DEFAULT_AMBIENT = 0.1f;
constexpr float DEFAULT_DIFFUSE = 0.8f;
constexpr float DEFAULT_SPECULAR = 1.0f;

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

	// point light
	float constant;
	float linear;
	float quadratic;

	// spot light (cutoff angles in degrees)
	glm::vec3 direction;
	float cutOff;
	float outerCutOff;

	LightSource();
	static LightSource Directional(const glm::vec3& direction);
	static LightSource Point(const glm::vec3& position, float constant, float linear, float quadratic);
	static LightSource Spotlight(const glm::vec3& position, const glm::vec3& direction, float cutOff, float outerCutOff);

	void overrideIntensities(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);
};
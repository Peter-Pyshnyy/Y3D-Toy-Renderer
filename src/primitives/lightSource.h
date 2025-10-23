#pragma once
#include <glm/glm.hpp>

enum LightType {
	POINT,
	DIRECTIONAL,
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
		glm::vec3 specular = glm::vec3(1.0f)): 
		type(POINT),
		position(position),
		ambient(ambient),
		diffuse(diffuse),
		specular(specular),
		constant(0.0f),
		linear(0.0f),
		quadratic(0.0f),
		direction(0.0f),
		cutOff(0.0f),
		outerCutOff(0.0f)
	{
	}

	void constructAsPointLight(float constant, float linear, float quadratic) {
		this->type = POINT;
		this->constant = constant;
		this->linear = linear;
		this->quadratic = quadratic;
		
		this->direction = glm::vec3(0.0f);
		this->cutOff = 0.0f;
		this->outerCutOff = 0.0f;
	}

	void constructAsDirectionalLight(glm::vec3 direction) {
		this->type = DIRECTIONAL;
		this->direction = direction;
		this->constant = 0.0f;
		this->linear = 0.0f;
		this->quadratic = 0.0f;
		this->cutOff = 0.0f;
		this->outerCutOff = 0.0f;
	}

	void constructAsSpotLight(glm::vec3 direction, float cutOff, float outerCutOff, float constant, float linear, float quadratic) {
		this->type = SPOT;
		this->direction = direction;
		this->cutOff = cutOff;
		this->outerCutOff = outerCutOff;
		this->constant = constant;
		this->linear = linear;
		this->quadratic = quadratic;
	}
};
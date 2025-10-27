#include "LightSource.h"

glm::i8vec3 LightSource::lightCount = glm::i8vec3(0);

LightSource::LightSource(
    glm::vec3 position,
    glm::vec3 ambient,
    glm::vec3 diffuse,
    glm::vec3 specular
)
    : type(POINT),
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

void LightSource::constructAsDirectionalLight(glm::vec3 direction) {
	this->type = DIRECTIONAL;
	this->direction = direction;
	this->constant = 0.0f;
	this->linear = 0.0f;
	this->quadratic = 0.0f;
	this->cutOff = 0.0f;
	this->outerCutOff = 0.0f;

	LightSource::lightCount.x = 1; //only one directional light allowed
}

void LightSource::constructAsPointLight(float constant, float linear, float quadratic) {
	this->type = POINT;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;

	this->direction = glm::vec3(0.0f);
	this->cutOff = 0.0f;
	this->outerCutOff = 0.0f;

	
}


void LightSource::constructAsSpotlight(glm::vec3 direction, float cutOff, float outerCutOff, float constant, float linear, float quadratic) {
	this->type = SPOT;
	this->direction = direction;
	this->cutOff = cutOff;
	this->outerCutOff = outerCutOff;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}

bool LightSource::incrementLightCount(LightType type) {
	switch (type) {
	case DIRECTIONAL: {
		lightCount.x = 1; //only one directional light allowed
		return true;
	}
	case POINT: {
		if (LightSource::lightCount.y < MAX_POINTLIGHTS) {
			LightSource::lightCount.y += 1;
			return true;
		}
		return false;
	}
	case SPOT: {
		if (LightSource::lightCount.z < MAX_SPOTLIGHTS) {
			LightSource::lightCount.z += 1;
			return true;
		}
		return false;
	}
	default:
		return false;
	}
}

glm::i8vec3 LightSource::getLightCount() {
	return LightSource::lightCount;
};
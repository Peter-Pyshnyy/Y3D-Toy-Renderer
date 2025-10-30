#include "LightSource.h"

LightSource::LightSource() :
	type(DIRECTIONAL),
	position(0.0f),
	ambient(0.0f),
	diffuse(0.0f),
	specular(0.0f),
	constant(1.0f),
	linear(0.0f),
	quadratic(0.0f),
	direction(0.0f),
	cutOff(0.0f),
	outerCutOff(0.0f)
{
}

LightSource LightSource::Directional(const glm::vec3& direction) {
	LightSource l;
	l.ambient = glm::vec3(DEFAULT_AMBIENT);
	l.diffuse = glm::vec3(DEFAULT_DIFFUSE);
	l.specular = glm::vec3(DEFAULT_SPECULAR);
	l.type = DIRECTIONAL;
	l.direction = direction;
	return l;
}

LightSource LightSource::Point(const glm::vec3& position, float constant, float linear, float quadratic) {
	LightSource l;
	l.ambient = glm::vec3(DEFAULT_AMBIENT);
	l.diffuse = glm::vec3(DEFAULT_DIFFUSE);
	l.specular = glm::vec3(DEFAULT_SPECULAR);
	l.type = POINT;
	l.position = position;
	l.constant = constant;
	l.linear = linear;
	l.quadratic = quadratic;
	return l;
}

LightSource LightSource::Spotlight(const glm::vec3& position, const glm::vec3& direction, float cutOff, float outerCutOff) { // cutOffs are in degrees
	LightSource l;
	l.ambient = glm::vec3(DEFAULT_AMBIENT);
	l.diffuse = glm::vec3(DEFAULT_DIFFUSE);
	l.specular = glm::vec3(DEFAULT_SPECULAR);
	l.type = SPOT;
	l.position = position;
	l.direction = direction;
	l.cutOff = glm::cos(glm::radians(cutOff));
	l.outerCutOff = glm::cos(glm::radians(outerCutOff));
	return l;
}

void LightSource::overrideIntensities(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular) {
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
}
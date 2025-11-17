#pragma once
#include "../sceneNode.h"
#include <memory>
#include "../../lightData.h"

class SpotlightNode : public SceneNode {
public:
	SpotlightNode(const std::string& name, glm::vec3& pos = glm::vec3(0.0f), glm::vec3& dir = glm::vec3(0.0f, 0.0f, -1.0f))
		: SceneNode(name), properties{} {
		properties.position = pos;
		properties.direction = dir;
	}
	virtual ~SpotlightNode() = default;

	void updateLocalTransform() override {
		SceneNode::updateLocalTransform();
		glm::mat4 posTransform = getWorldTransform();
		glm::mat4 dirTransform = glm::transpose(glm::inverse(posTransform));
		properties.position = glm::vec3(posTransform * glm::vec4(properties.position, 1.0f));
		properties.direction = glm::normalize(glm::vec3(dirTransform * glm::vec4(properties.direction, 0.0f)));
	}

	void submit(Renderer& renderer) const override {
		renderer.submit(properties);
	}
	Spotlight properties;
};
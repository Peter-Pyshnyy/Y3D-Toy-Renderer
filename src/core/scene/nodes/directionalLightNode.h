#pragma once
#include "../sceneNode.h"
#include <memory>
#include "../../lightData.h"

class DirectionalLightNode : public SceneNode {
public:
	DirectionalLightNode(glm::vec3& dir = glm::vec3(1.0f, -1.0f, 0.0f))
		: SceneNode("directional light"), properties.direction(dir) {
	}
	virtual ~DirectionalLightNode() = default;

	void SceneNode::updateTransform() override {
		SceneNode::updateTransform();
		glm::mat4 dirTransform = glm::transpose(glm::inverse(getWorldTransform()));
		properties.direction = glm::normalize(glm::vec3(dirTransform * glm::vec4(properties.direction, 0.0f)));
	}

	void submit(Renderer& renderer) const override {
		renderer.submit(*properties, getWorldTransform());
	}
	DirectionalLight properties;
};
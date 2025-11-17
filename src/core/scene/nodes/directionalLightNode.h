#pragma once
#include "../sceneNode.h"
#include <memory>
#include "../../lightData.h"

constexpr glm::vec3 DEFAULT_DIRECTION = glm::vec3(0.0f, -1.0f, 0.0f);

class DirectionalLightNode : public SceneNode {
public:
	DirectionalLightNode(glm::vec3& dir = glm::vec3(1.0f, -1.0f, 0.0f))
		: SceneNode("directional light"), properties{} {
		properties.direction = dir;
	}
	virtual ~DirectionalLightNode() = default;

	void updateWorldTransform(const glm::mat4& parentsWorld) override {
		if (dirty) {
			SceneNode::updateWorldTransform(parentsWorld);
			updateProperties();
		} else {
			SceneNode::updateWorldTransform(parentsWorld);
		}
	}

	void updateProperties() {
		glm::mat4 dirTransform = glm::transpose(glm::inverse(worldMatrix));
		properties.direction = glm::normalize(dirTransform * glm::vec4(DEFAULT_DIRECTION, 0.0f));
	}

	void submit(Renderer& renderer) const override {
		renderer.submit(properties);
	}
	DirectionalLight properties;
};
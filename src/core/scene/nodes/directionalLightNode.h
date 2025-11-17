#pragma once
#include "../sceneNode.h"
#include <memory>
#include "../../lightData.h"

constexpr glm::vec3 DEFAULT_DIRECTION = glm::vec3(0.0f, -1.0f, 0.0f);

class DirectionalLightNode : public SceneNode {
public:
	DirectionalLightNode()
		: SceneNode("directional light"), properties{} {
		properties.direction = DEFAULT_DIRECTION;
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
		glm::mat3 basis(worldMatrix);
		basis[0] = glm::normalize(basis[0]);
		basis[1] = glm::normalize(basis[1]);
		basis[2] = glm::normalize(basis[2]);
		properties.direction = glm::normalize(basis * DEFAULT_DIRECTION);
	}

	void submit(Renderer& renderer) const override {
		renderer.submit(properties);
	}
	DirectionalLight properties;
};
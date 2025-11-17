#pragma once
#include "../sceneNode.h"
#include <memory>
#include "../../lightData.h"

constexpr glm::vec3 DEFAULT_SPOTLIGHT_DIRECTION = glm::vec3(0.0f, 0.0f, -1.0f);

class SpotlightNode : public SceneNode {
public:
	SpotlightNode(const std::string& name, glm::vec3& pos = glm::vec3(0.0f))
		: SceneNode(name), properties{} {
		properties.position = pos;
		properties.direction = DEFAULT_SPOTLIGHT_DIRECTION;
	}
	virtual ~SpotlightNode() = default;

	void updateWorldTransform(const glm::mat4& parentsWorld) override {
		if (dirty) {
			SceneNode::updateWorldTransform(parentsWorld);
			updateProperties();
		}
		else {
			SceneNode::updateWorldTransform(parentsWorld);
		}
	}

	void updateProperties() {
		glm::mat4 dirTransform = glm::transpose(glm::inverse(worldMatrix));
		properties.position = (worldMatrix * glm::vec4(transform.position, 1.0f));
		properties.direction = glm::normalize(dirTransform * glm::vec4(DEFAULT_SPOTLIGHT_DIRECTION, 0.0f));
	}

	void submit(Renderer& renderer) const override {
		renderer.submit(properties);
	}
	Spotlight properties;
};
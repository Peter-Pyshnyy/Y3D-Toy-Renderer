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
		properties.position = glm::vec3(worldMatrix[3]); // extracts translation

		// cancel out scaling
		glm::mat3 basis(worldMatrix);
		basis[0] = glm::normalize(basis[0]);
		basis[1] = glm::normalize(basis[1]);
		basis[2] = glm::normalize(basis[2]);
		properties.direction = glm::normalize(basis * DEFAULT_SPOTLIGHT_DIRECTION);


	}

	void submit(Renderer& renderer) const override {
		renderer.submit(properties);
	}
	Spotlight properties;
};
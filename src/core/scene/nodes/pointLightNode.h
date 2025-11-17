#pragma once
#include "../sceneNode.h"
#include <memory>
#include "../../lightData.h"

class PointLightNode : public SceneNode {
public:
	PointLightNode(const std::string& name, glm::vec3& pos = glm::vec3(0.0f))
		: SceneNode(name), properties{} {
		properties.position = pos;
	}
	virtual ~PointLightNode() = default;

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
	}

	void submit(Renderer& renderer) const override {
		renderer.submit(properties);
	}
	PointLight properties;
};
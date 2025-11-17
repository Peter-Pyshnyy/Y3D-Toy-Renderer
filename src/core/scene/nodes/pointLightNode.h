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

	void updateWorldTransform() override {
		SceneNode::updateWorldTransform();
		updateProperties();
	}

	void updateProperties() {
		properties.position = worldMatrix * glm::vec4(transform.position, 1.0f);
	}

	void submit(Renderer& renderer) const override {
		renderer.submit(properties);
	}
	PointLight properties;
};
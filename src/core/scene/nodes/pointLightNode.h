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

	void updateTransform() override {
		SceneNode::updateTransform();
		glm::mat4 posTransform = getWorldTransform();
		properties.position = glm::vec3(posTransform * glm::vec4(properties.position, 0.0f));
	}

	void submit(Renderer& renderer) const override {
		renderer.submit(properties);
	}
	PointLight properties;
};
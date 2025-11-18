#pragma once
#include "../sceneNode.h"
#include <memory>
#include "../../../geometry/model.h" 

class ModelNode : public SceneNode {
public:
	ModelNode(const std::string& name, std::shared_ptr<Model>& model) : SceneNode(name), model(model) {}
	virtual ~ModelNode() = default;
	void setModel(const std::shared_ptr<Model>& model) { this->model = model; }
	std::shared_ptr<Model> getModel() const { return model; }

	void updateWorldTransform(const glm::mat4& parentsWorld) override {
		if (dirty) {
			SceneNode::updateWorldTransform(parentsWorld);
			normalMatrix = glm::transpose(glm::inverse(glm::mat3(worldMatrix)));
		}
		else {
			SceneNode::updateWorldTransform(parentsWorld);
		}
	}

	void submit(Renderer& renderer) const override {
		renderer.submit(*model, worldMatrix, normalMatrix); 
	}
private:
	std::shared_ptr<Model> model;
	glm::mat4 normalMatrix = glm::mat4(1.0f);
};
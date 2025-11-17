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
	void submit(Renderer& renderer) const override {
		renderer.submit(*model, worldMatrix); 
	}
private:
	std::shared_ptr<Model> model;
};
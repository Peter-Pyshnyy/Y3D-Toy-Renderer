#pragma once
#include "../sceneNode.h"
#include <memory>
#include "../../../geometry/model.h" 

class GeometryNode : public SceneNode {
public:
	GeometryNode(const std::string& name, std::shared_ptr<Model>& model) : SceneNode(name), model(model) {}
	virtual ~GeometryNode() = default;
	void setModel(const std::shared_ptr<Model>& model) { this->model = model; }
	std::shared_ptr<Model> getModel() const { return model; }
	void submit(Renderer& renderer) const override {
		renderer.submitModel(*model, getWorldTransform());
	}
private:
	std::shared_ptr<Model> model;
};
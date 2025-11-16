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
		renderer.submit(*model, getWorldTransform()); 
		// TODO: strore transforms locally and pass them down the scene graph
		// when doing that, make sure to multiply parent's world transform with local transform on addChild
	}
private:
	std::shared_ptr<Model> model;
};
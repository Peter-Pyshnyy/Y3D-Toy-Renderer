#pragma once
#include "../sceneNode.h"
#include <memory>
#include "../../../geometry/model.h" 

class GeometryNode : public SceneNode {
public:
	GeometryNode(const std::string& name, const std::shared_ptr<Model>& model);
	virtual ~GeometryNode() = default;
	void setModel(const std::shared_ptr<Model>& model) { this->model = model; }
	std::shared_ptr<Model> getModel() const { return model; }
	void draw();
	bool visible = true;
private:
	std::shared_ptr<Model> model;
};
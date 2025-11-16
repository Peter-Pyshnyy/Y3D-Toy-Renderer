#pragma once
#include "../sceneNode.h"
#include <memory>
#include "../../../geometry/primitive.h" 

class PrimitiveNode : public SceneNode {
public:
	PrimitiveNode(const std::string& name, std::shared_ptr<Primitive>& primitive = std::make_shared<Primitive>(PrimitiveType::Cube))
		: SceneNode(name), primitive(primitive) {}
	virtual ~PrimitiveNode() = default;
	void setPrimitive(const std::shared_ptr<Primitive>& primitive) { this->primitive = primitive; }
	std::shared_ptr<Primitive> getPrimitive() const { return primitive; }
	void submit(Renderer& renderer) const override {
		renderer.submit(*primitive, getWorldTransform());
	}
private:
	std::shared_ptr<Primitive> primitive;
};
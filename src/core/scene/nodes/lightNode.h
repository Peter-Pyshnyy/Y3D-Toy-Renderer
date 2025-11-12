#pragma once
#include "../sceneNode.h"
#include "../../lightSource.h"

class LightNode : public SceneNode {
public:
	LightNode(const std::string& name, const LightSource& light) : SceneNode(name), lightSource(light) {}
	virtual ~LightNode() = default;
	LightSource& getLightSource() { return lightSource; }
	void submit(Renderer& renderer) const override {
		//renderer.submitLight(light, getWorldTransform());
	}
private:
	LightSource& lightSource;
};
#pragma once
#include "../sceneNode.h"
#include "../../lightSource.h"

class LightNode : public SceneNode {
public:
	LightNode(const std::string& name, LightSource& light);
	virtual ~LightNode() = default;
	LightSource& getLightSource() { return lightSource; }
private:
	LightSource& lightSource;
};
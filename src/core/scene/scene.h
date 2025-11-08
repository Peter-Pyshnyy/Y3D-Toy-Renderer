#pragma once

#include <memory>
#include <vector>
#include "sceneNode.h"

class Scene {
public:
	Scene(); // TODO: make ctor create a default root node
	~Scene();
	void draw();
private:
	std::unique_ptr<SceneNode> rootNode;
};
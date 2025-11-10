#pragma once

#include <memory>
#include <vector>
#include "sceneNode.h"

class Scene {
public:
	Scene(); // TODO: make ctor create a default root node

	// Rule of 5
	~Scene();
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
	Scene(Scene&&) noexcept = default;
	Scene& operator=(Scene&&) noexcept = default;

	void draw() const;
	SceneNode& getRoot() const;
private:
	std::unique_ptr<SceneNode> root;
};
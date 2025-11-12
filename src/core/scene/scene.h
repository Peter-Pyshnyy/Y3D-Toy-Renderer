#pragma once

#include <memory>
#include <vector>
#include "sceneNode.h"

class Renderer;

class Scene {
public:
	Scene(); // TODO: make ctor create a default root node

	// Rule of 5
	~Scene();
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
	Scene(Scene&&) noexcept = default;
	Scene& operator=(Scene&&) noexcept = default;

	void submit(Renderer& renderer) const;
	SceneNode& getRoot() const;
private:
	std::unique_ptr<SceneNode> root;
	void submitRecursive(const SceneNode& node, Renderer& renderer) const;
};
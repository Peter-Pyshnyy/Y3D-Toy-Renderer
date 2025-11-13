#include "scene.h"
#include "../renderer.h"

Scene::Scene() : root(std::make_unique<SceneNode>("root")) {}

Scene::~Scene() {
	root.reset();
}

Scene::Scene(Scene&& other) noexcept : root(std::move(other.root)) {}

Scene& Scene::operator=(Scene&& other) noexcept {
	if (this != &other) {
		root = std::move(other.root);
	}
	return *this;
}

void Scene::submit(Renderer& renderer) const {
	renderer.clearDrawList(); //remove later
	submitRecursive(*root, renderer);
}

void Scene::submitRecursive(const SceneNode& node, Renderer& renderer) const {
	node.submit(renderer);

	for (const auto& child : node.children)
		submitRecursive(*child, renderer);
}

SceneNode* Scene::getRoot() const {
	return root.get();
}
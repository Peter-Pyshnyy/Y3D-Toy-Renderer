#include "scene.h"

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

void draw() const {
	root->draw(); // change later 
}

SceneNode* Scene::getRoot() const {
	return root.get();
}
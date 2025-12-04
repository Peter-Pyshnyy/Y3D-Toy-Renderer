#define GLM_ENABLE_EXPERIMENTAL // for glm::gtx/euler_angles

#include "sceneNode.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "../renderer.h"

SceneNode::SceneNode(const std::string& name)
    : name(name), parent(nullptr), children(), modelMatrix(1.0f), worldMatrix(1.0f), transform()
{
}

SceneNode::~SceneNode() {
	parent = nullptr;
}

void SceneNode::addChild(std::unique_ptr<SceneNode> child) {
    if (child) {
        child->parent = this;
        children.emplace_back(std::move(child));
		children.back()->propagateDirty();
    }
}

std::unique_ptr<SceneNode> SceneNode::removeChild(SceneNode* child) {
    auto found = std::find_if(children.begin(), children.end(),
        [child](const std::unique_ptr<SceneNode>& p) { return p.get() == child; });

    if (found == children.end())
		std::cout << "Child not found in SceneNode::removeChild" << "\n";

	std::unique_ptr<SceneNode> result = std::move(*found); // found is an iterator to unique_ptr, thus *found is the unique_ptr
    children.erase(found);
    result->parent = nullptr; // if you track parent pointers
    return result; // ownership transferred to caller
}

int SceneNode::getChildCount() const {
    return static_cast<int>(children.size());
}

SceneNode* SceneNode::getChild(int index) const {
        if (index < 0 || index >= static_cast<int>(children.size()))
            throw std::out_of_range("Index out of range in SceneNode::getChild");
		return children[index].get();
}

SceneNode* SceneNode::getParent() const {
    return parent;
}

void SceneNode::setModelMatrix(const glm::mat4& matrix) {
    modelMatrix = matrix;
	updateTransformComponents();
	propagateDirty();
}

void SceneNode::translate(const glm::vec3& delta) {
    transform.position = transform.position + delta;
    updateLocalTransform();
}

// in world space
void SceneNode::rotate(const glm::vec3& euler) {
	transform.rotation = transform.rotation + euler;
    updateLocalTransform();
}

void SceneNode::scale(float scaleFactor) {
    transform.scale = transform.scale * scaleFactor;
    updateLocalTransform();
}

glm::mat4 SceneNode::getLocalTransform() const {
    return modelMatrix;
}

glm::mat4 SceneNode::getWorldTransform() const {
	return worldMatrix;
}

void SceneNode::propagateDirty() {
    dirty = true;
    for (auto& child : children) {
        child->propagateDirty();
    }
}

void SceneNode::updateLocalTransform() {
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), transform.position);
    glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.x), glm::vec3(1, 0, 0));
    glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.y), glm::vec3(0, 1, 0));
    glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.z), glm::vec3(0, 0, 1));
    glm::mat4 rotationMatrix = rotationZ * rotationY * rotationX;
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(transform.scale));
	modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	propagateDirty();
}

void SceneNode::updateTransformComponents() {
	transform.position = glm::vec3(modelMatrix[3]);
	transform.scale = glm::length(glm::vec3(modelMatrix[0])); // assuming uniform scaling
	glm::vec3 euler; 
    glm::extractEulerAngleXYZ(modelMatrix, euler.x, euler.y, euler.z);
	transform.rotation = glm::degrees(euler);
}

// propagate world transform update to children
void SceneNode::updateWorldTransform(const glm::mat4& parentsWorld) {
    if (dirty){
	    worldMatrix = parentsWorld * modelMatrix;
		dirty = false;
    }

    if (children.empty()) {
        return;
	}

    for (auto& child : children) {
        child->updateWorldTransform(worldMatrix);
	}
}

void SceneNode::submit(Renderer& renderer) const {}

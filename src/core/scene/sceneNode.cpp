#include "sceneNode.h"

SceneNode::SceneNode(const std::string& name)
	: name(name), parent(nullptr), children(), localTransform()
{
}

SceneNode::~SceneNode() {
	for (SceneNode* child : children) {
		delete child;
	}
	parent = nullptr;
}

void SceneNode::addChild(std::unique_ptr<SceneNode> child) {
    if (child) {
        child->parent = this;
        children.emplace_back(std::move(child));
    }
}

std::unique_ptr<SceneNode> SceneNode::removeChild(SceneNode* child) {
    auto found = std::find_if(children.begin(), children.end(),
        [child](const std::unique_ptr<SceneNode>& p) { return p.get() == child; });

    if (found == children.end())
        throw std::runtime_error("Child not found in SceneNode::removeChild");

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


#include "sceneNode.h"

SceneNode::SceneNode(const std::string& name)
	: name(name), parent(nullptr), children(), localTransform()
{
}

SceneNode::~SceneNode()
{
	for (SceneNode* child : children)
	{
		delete child;
	}
	parent = nullptr;
}
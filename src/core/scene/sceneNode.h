#pragma once
#include <memory>
#include <vector>
#include <string>
#include <glm/glm.hpp>

class SceneNode {
public:
		SceneNode(const std::string& name);
		virtual ~SceneNode();

		void addChild(std::unique_ptr<SceneNode> child);
		void removeChild(SceneNode* child);
		int getChildCount() const;
		SceneNode* getChild(int index) const;
		SceneNode* getParent() const;

		void setLocalTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
		void setLocalTransform(const glm::mat4& matrix);
		void translate(const glm::vec3& delta);
		void rotate(const glm::vec3& euler);
		void scale(const glm::vec3& factor);
		glm::mat4 getLocalTransform() const;
		glm::mat4 getWorldTransform() const;
protected:
	glm::mat4 localTransform;
	SceneNode* parent;
	std::vector<std::unique_ptr<SceneNode>> children;
};
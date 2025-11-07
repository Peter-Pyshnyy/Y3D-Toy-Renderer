#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>

struct Transform {
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

class SceneNode {
public:
		SceneNode();
		virtual ~SceneNode();
		virtual void update(float deltaTime) = 0;
		virtual void draw() = 0;

		void addChild(std::unique_ptr<SceneNode> child);
		void removeChild(SceneNode* child);
		SceneNode* getChild(int index) const;
		SceneNode* getParent() const;
		int getChildCount() const;
		void setLocalTransform(const Transform& transform);
		void setLocalTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
		void setLocalTransform(const glm::mat4& matrix);

		glm::mat4 getLocalTransform() const;
		glm::mat4 getWorldTransform() const;
protected:
	glm::mat4 localTransform;
	SceneNode* parent;
	std::vector<std::unique_ptr<SceneNode>> children;
};
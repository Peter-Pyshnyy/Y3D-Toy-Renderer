#pragma once
#include <memory>
#include <vector>
#include <string>
#include <glm/glm.hpp>

class Renderer;

// added to ensure correct transformation order on update
struct Transform {
	glm::vec3 position;
	glm::vec3 rotation; // Euler angles in radians
	glm::vec3 scale;
};

class SceneNode {
public:
		Transform transform;

		SceneNode(const std::string& name);
		virtual ~SceneNode();
		// TODO: add rule of 5 AND empty virtual draw() method

		void addChild(std::unique_ptr<SceneNode> child);
		std::unique_ptr<SceneNode> removeChild(SceneNode* child);
		int getChildCount() const;
		SceneNode* getChild(int index) const;
		SceneNode* getParent() const;

		void setModelMatrix(const glm::mat4& matrix);
		void translate(const glm::vec3& delta);
		void rotate(const glm::vec3& euler);
		void scale(const glm::vec3& factor);
		glm::mat4 getLocalTransform() const;
		glm::mat4 getWorldTransform() const;

		virtual void submit(Renderer& renderer) const;
protected:
	SceneNode* parent;
	std::vector<std::unique_ptr<SceneNode>> children;
	glm::mat4 modelMatrix;
	void updateTransform();
};
#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "../core/shader.h"

//modified https://learnopengl.com/Model-Loading/Mesh
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture {
	unsigned id;
	std::string type;
	std::string path;
};

class Mesh {
public:
	Mesh() : EBO(0), VAO(0), VBO(0) {} // Default constructor for primitives
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	// Rule of 5
	virtual ~Mesh();
	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;
	Mesh(Mesh&& other) noexcept;
	Mesh& operator=(Mesh&& other) noexcept;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	virtual void Draw(Shader& shader) const;
protected:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};
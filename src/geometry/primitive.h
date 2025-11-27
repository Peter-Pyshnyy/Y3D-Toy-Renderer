#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "../core/shader.h"
#include "mesh.h"

enum class PrimitiveType {
    Plane,
    Cube
};

class Primitive : public Mesh {
public:
    PrimitiveType type;
	glm::vec3 color;
    std::vector<Vertex> vertices;
	Primitive(PrimitiveType type, float scale = 1.0f, glm::vec3& color = glm::vec3(0.8f)); // for uniform scaling
    Primitive(PrimitiveType type, float scaleX, float scaleY, float scaleZ, glm::vec3& color);
    void Draw(Shader& shader) const override;
private:
    void setupMesh();
    static std::vector<Vertex> generateVertices(PrimitiveType type, float sx, float sy, float sz);
    static std::vector<unsigned int> generateIndices(PrimitiveType type, float sx, float sz);
};

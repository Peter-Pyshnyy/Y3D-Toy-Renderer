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
    Primitive(PrimitiveType type, glm::vec3 color = glm::vec3(0.8f), float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.0f);
private:
    void setupMesh();
    static std::vector<Vertex> generateVertices(PrimitiveType type, float sx, float sy, float sz);
    static std::vector<unsigned int> generateIndices(PrimitiveType type, float sx, float sz);
};

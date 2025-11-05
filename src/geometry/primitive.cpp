#include "primitive.h"
#include <iostream>

Primitive::Primitive(PrimitiveType type, glm::vec3 color, float scaleX, float scaleY, float scaleZ)
    : Mesh(), type(type), color(color) {
    this->vertices = generateVertices(type, scaleX, scaleY, scaleZ);
	this->indices = generateIndices(type, scaleX, scaleZ);
    this->textures = {};

    setupMesh();
}

Primitive::Primitive(PrimitiveType type, glm::vec3 color, float scale)
: Mesh(), type(type), color(color) {
    this->vertices = generateVertices(type, scale, scale, scale);
    this->indices = generateIndices(type, scale, scale);
    this->textures = {};

    setupMesh();
}

std::vector<Vertex> Primitive::generateVertices(PrimitiveType type, float sx, float sy, float sz) {
    std::vector<Vertex> vertices;

    switch (type) {
    case PrimitiveType::Plane: {
        // grid of vertices spaced 1 unit apart, centered at origin, facing +Y
        int halfX = static_cast<int>(sx);
        int halfZ = static_cast<int>(sz);

        for (int z = -halfZ; z <= halfZ; ++z) {
            for (int x = -halfX; x <= halfX; ++x) {
                glm::vec3 pos((float)x, 0.0f, (float)z);
                glm::vec3 normal(0.0f, 1.0f, 0.0f);
                glm::vec2 uv(
                    (float)(x + halfX) / (2.0f * halfX),
                    (float)(z + halfZ) / (2.0f * halfZ)
                );
                vertices.push_back({ pos, normal, {} });
            }
        }
        break;
    }
    case PrimitiveType::Cube: {
        // unit cube centered on origin
        const glm::vec3 normals[] = {
            { 0,  0,  1}, { 0,  0, -1},
            { 1,  0,  0}, {-1,  0,  0},
            { 0,  1,  0}, { 0, -1,  0}
        };
        const glm::vec3 positions[6][4] = {
            // +Z
            {{-1, -1,  1}, { 1, -1,  1}, { 1,  1,  1}, {-1,  1,  1}},
            // -Z
            {{ 1, -1, -1}, {-1, -1, -1}, {-1,  1, -1}, { 1,  1, -1}},
            // +X
            {{ 1, -1,  1}, { 1, -1, -1}, { 1,  1, -1}, { 1,  1,  1}},
            // -X
            {{-1, -1, -1}, {-1, -1,  1}, {-1,  1,  1}, {-1,  1, -1}},
            // +Y
            {{-1,  1,  1}, { 1,  1,  1}, { 1,  1, -1}, {-1,  1, -1}},
            // -Y
            {{-1, -1, -1}, { 1, -1, -1}, { 1, -1,  1}, {-1, -1,  1}},
        };

        for (int f = 0; f < 6; ++f) {
            for (int v = 0; v < 4; ++v) {
                glm::vec3 pos = positions[f][v] * 0.5f * glm::vec3(sx, sy, sz);
                vertices.push_back({ pos, normals[f], {} });
            }
        }
        break;
    }
    }

    return vertices;
}
std::vector<unsigned int> Primitive::generateIndices(PrimitiveType type, float sx, float sz) {
    std::vector<unsigned int> indices;

    switch (type) {
    case PrimitiveType::Plane: {
        int halfX = static_cast<int>(sx);
        int halfZ = static_cast<int>(sz);
        int vertsX = (halfX * 2) + 1;
        int vertsZ = (halfZ * 2) + 1;

        for (int z = 0; z < vertsZ - 1; ++z) {
            for (int x = 0; x < vertsX - 1; ++x) {
                unsigned int topLeft = z * vertsX + x;
                unsigned int topRight = topLeft + 1;
                unsigned int bottomLeft = (z + 1) * vertsX + x;
                unsigned int bottomRight = bottomLeft + 1;

                indices.insert(indices.end(),
                    { topLeft, bottomLeft, topRight,
                      topRight, bottomLeft, bottomRight });
            }
        }
        break;
    }
    case PrimitiveType::Cube: {
        for (unsigned int f = 0; f < 6; ++f) {
            unsigned int base = f * 4;
            indices.insert(indices.end(),
                { base, base + 1, base + 2, base + 2, base + 3, base });
        }
        break;
    }
    }

    return indices;
}

void Primitive::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glBindVertexArray(0);
}

void Primitive::Draw(Shader& shader) const {
	shader.setVec3("u_color", color);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
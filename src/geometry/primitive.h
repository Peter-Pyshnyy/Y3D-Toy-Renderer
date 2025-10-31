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

    Primitive(PrimitiveType type,
        float scaleX = 1.0f,
        float scaleY = 1.0f,
        float scaleZ = 1.0f)
        : Mesh(generateVertices(type, scaleX, scaleY, scaleZ),
            generateIndices(type, scaleX, scaleZ),
            {}), // empty texture list
        type(type)
    {
    }

private:
    static std::vector<Vertex> generateVertices(PrimitiveType type,
        float sx,
        float sy,
        float sz) {
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
                    vertices.push_back({ pos, normal, uv });
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

            const glm::vec2 uvs[] = { {0,0},{1,0},{1,1},{0,1} };

            for (int f = 0; f < 6; ++f) {
                for (int v = 0; v < 4; ++v) {
                    glm::vec3 pos = positions[f][v] * 0.5f * glm::vec3(sx, sy, sz);
                    vertices.push_back({ pos, normals[f], uvs[v] });
                }
            }
            break;
        }
        }

        return vertices;
    }

    static std::vector<unsigned int> generateIndices(PrimitiveType type,
        float sx,
        float sz) {
        std::vector<unsigned int> indices;

        switch (type) {
        case PrimitiveType::Plane: {
            int halfX = static_cast<int>(sx);
            int halfZ = static_cast<int>(sz);
            int vertsX = (halfX * 2) + 1;
            int vertsZ = (halfZ * 2) + 1;

            for (int z = 0; z < vertsZ - 1; ++z) {
                for (int x = 0; x < vertsX - 1; ++x) {
                    int topLeft = z * vertsX + x;
                    int topRight = topLeft + 1;
                    int bottomLeft = (z + 1) * vertsX + x;
                    int bottomRight = bottomLeft + 1;

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
};

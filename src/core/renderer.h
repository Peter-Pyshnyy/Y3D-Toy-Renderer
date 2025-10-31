#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "shader.h"
#include "camera.h"
#include "lightSource.h"
#include "../geometry/model.h"
#include "../geometry/primitive.h"

class Renderer {
public:
    Renderer(int width, int height);
    ~Renderer();

    void init();
    void renderFrame(const Camera& camera, float time, float deltaTime);

    void addLight(const LightSource& light);
    void addModel(const std::string& name);
    void addPrimitive(const Primitive& primitive);

private:
    int width, height;
	bool hasDirectionalLight = false;
    Shader shader;
    LightSource dirLight;
    std::vector<LightSource> pointLights;
    std::vector<LightSource> spotlights;
    std::vector<Model> models;
    std::vector<Primitive> primitives;

    void setupShader();
    void updateShaderLights();
};
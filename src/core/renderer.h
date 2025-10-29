#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "shader.h"
#include "camera.h"
#include "lightSource.h"
#include "../geometry/model.h"

class Renderer {
public:
    Renderer(int width, int height);
    ~Renderer();

    void init();
    void renderFrame(const Camera& camera, float time, float deltaTime);

    void addLight(const LightSource& light);
    void Renderer::addModel(const std::string& name);

private:
    int width, height;
    Shader shader;
    std::vector<LightSource> lights;
    std::vector<Model> models;

    void setupShader();
    void updateShaderLights();
};
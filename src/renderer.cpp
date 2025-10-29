// Renderer.cpp
#include "Renderer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <filesystem>
#include "paths.h"

Renderer::Renderer(int w, int h) : width(w), height(h) {}

Renderer::~Renderer() {
    shader.clear(); 
}

void Renderer::init() {
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    setupShader();
}

void Renderer::setupShader() {
    std::filesystem::path vert = std::filesystem::path(SHADER_DIR) / "shader.vert";
    std::filesystem::path frag = std::filesystem::path(SHADER_DIR) / "shader.frag";
    shader.loadShaderProgramFromFile(vert.string().c_str(), frag.string().c_str());
    glLinkProgram(shader.id);
    shader.bind();
}

void Renderer::addLight(const LightSource& light) {
    lights.push_back(light);
}

void Renderer::addModel(const std::string& name) {
    // assuming models stored in MODEL_DIR/name/name.obj
	std::filesystem::path path = std::filesystem::path(MODEL_DIR) / std::filesystem::path(name + "/" + name + ".obj"); 
    models.emplace_back(path.string());
}

void Renderer::updateShaderLights() {
	//LightSource::resetLightCount(); // will break on any light source except directional light for now
    for (const auto& light : lights)
        shader.setLightSource(light);
    shader.setuVec3("lightCount", LightSource::getLightCount());
}

void Renderer::renderFrame(const Camera& camera, float time, float deltaTime) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.bind();
    updateShaderLights();

    glm::mat4 projection = glm::perspective(glm::radians(camera.getFov()),
        static_cast<float>(width) / height, 0.1f, 100.0f);
    shader.setMat4("u_proj", projection);
    shader.setMat4("u_view", camera.getWorldToViewMatrix());
    shader.setVec3("u_viewPos", camera.getPosition());
	shader.setFloat("u_time", time);

    for (auto& model : models)
        model.Draw(shader);
}

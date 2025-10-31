#include "renderer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <filesystem>
#include "../utils/paths.h"

Renderer::Renderer(int w, int h) : width(w), height(h), dirLight() {}

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
    switch (light.type) {
    case LightType::DIRECTIONAL: {
		hasDirectionalLight = true;
        dirLight = light;
        break;
    }
    case LightType::POINT: {
        if (pointLights.size() < MAX_POINTLIGHTS) {
            pointLights.push_back(light);
        } else {
            std::cout << "max pointLights reached!" << "\n";
        }
        break;
    }
    case LightType::SPOT: {
        if (spotlights.size() < MAX_SPOTLIGHTS) {
            spotlights.push_back(light);
        } else {
            std::cout << "max spotlights reached!" << "\n";
        }
        break;
    }
    default:
        break;
    }
    
}

void Renderer::addModel(const std::string& name) {
    // assuming models stored in MODEL_DIR/name/name.obj
	std::filesystem::path path = std::filesystem::path(MODEL_DIR) / std::filesystem::path(name + "/" + name + ".obj"); 
    models.emplace_back(path.string());
}

void Renderer::addPrimitive(const Primitive& primitive) {
    primitives.push_back(primitive);
}

void Renderer::updateShaderLights() {
    if (hasDirectionalLight) {
        shader.setDirectionalLight(dirLight);
    }

    for (std::size_t i = 0; i < pointLights.size(); i++) {
        shader.setPointLight(pointLights[i], i);
	}

    for (std::size_t i = 0; i < spotlights.size(); i++) {
        shader.setSpotlight(spotlights[i], i);
	}

    shader.setuVec3("lightCount", glm::uvec3(
        static_cast<unsigned int>(hasDirectionalLight),
        pointLights.size(),
        spotlights.size()
    ));
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

    for (Primitive primitive : primitives)
		primitive.Draw(shader);

    for (Model model : models)
        model.Draw(shader);
}

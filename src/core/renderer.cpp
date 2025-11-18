#include "renderer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <filesystem>
#include "../utils/paths.h"

constexpr int MAX_POINTLIGHTS = 16;
constexpr int MAX_SPOTLIGHTS = 16;

Renderer::Renderer(int w, int h) : width(w), height(h) {}

Renderer::~Renderer() {
    for (auto& [type, shader] : shaders) {
        shader.clear();
    }
}

void Renderer::init() {
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    setupShaders();
}

void Renderer::setupShaders() {
    std::filesystem::path vert = std::filesystem::path(SHADER_DIR) / "default.vert";
    std::filesystem::path frag = std::filesystem::path(SHADER_DIR) / "default.frag";
    shaders[ShaderType::Default] = Shader(vert.string().c_str(), frag.string().c_str());

	vert = std::filesystem::path(SHADER_DIR) / "primitive.vert";
	frag = std::filesystem::path(SHADER_DIR) / "primitive.frag";
	shaders[ShaderType::Primitive] = Shader(vert.string().c_str(), frag.string().c_str());
}

void Renderer::addModel(const std::string& name) {
    // assuming models stored in MODEL_DIR/name/name.obj
	std::filesystem::path path = std::filesystem::path(MODEL_DIR) / std::filesystem::path(name + "/" + name + ".obj"); 
    models.emplace_back(path.string());
}

void Renderer::addPrimitive(Primitive&& primitive) {
    primitives.push_back(std::move(primitive));
}

void Renderer::updateShaderLights(Shader& shader) {
    if (drawList.hasDirectionalLight) {
        shader.setDirectionalLight(drawList.directionalLight);
    }

    for (std::size_t i = 0; i < drawList.pointLights.size(); i++) {
        shader.setPointLight(drawList.pointLights[i], i);
	}

    for (std::size_t i = 0; i < drawList.spotlights.size(); i++) {
        shader.setSpotlight(drawList.spotlights[i], i);
	}

    shader.setuVec3("lightCount", glm::uvec3(
        static_cast<unsigned int>(drawList.hasDirectionalLight),
        drawList.pointLights.size(),
        drawList.spotlights.size()
    ));
}

// assumes shader is already bound
void Renderer::setUniforms(Shader& shader, const FrameData& frame, const glm::mat4& model, const glm::mat4& normal) {
    shader.setMat4("u_proj", frame.projection);
    shader.setMat4("u_view", frame.camera.getWorldToViewMatrix());
    shader.setMat4("u_model", model);
	shader.setMat4("u_normalMatrix", normal);
    shader.setVec3("u_viewPos", frame.camera.getPosition());
    shader.setFloat("u_time", frame.time);
    updateShaderLights(shader);
}

// model submission
void Renderer::submit(const Model& model, const glm::mat4& modelMatrix, const glm::mat4& normalMatrix) {
	drawList.models.push_back({ &model, modelMatrix, normalMatrix });
}

// primitive submission
void Renderer::submit(const Primitive& primitive, const glm::mat4& modelMatrix, const glm::mat4& normalMatrix) {
	drawList.primitives.push_back({ &primitive, modelMatrix, normalMatrix });
}

// directional light submission
void Renderer::submit(const DirectionalLight& l) {
	drawList.directionalLight = l;
	drawList.hasDirectionalLight = true;
}

// point light submission
void Renderer::submit(const PointLight& l) {
    if (drawList.pointLights.size() >= MAX_POINTLIGHTS) {
        std::cout << "Warning: Maximum number of point lights reached. Ignoring additional point light submission." << std::endl;
        return;
	}
    drawList.pointLights.push_back(l);
}

// spotlight submission
void Renderer::submit(const Spotlight& l) {
    if (drawList.spotlights.size() >= MAX_SPOTLIGHTS) {
        std::cout << "Warning: Maximum number of spotlights reached. Ignoring additional spotlight submission." << std::endl;
        return;
    }
    drawList.spotlights.push_back(l);
}

void Renderer::drawModels(const FrameData& frame) {
    Shader& shader = shaders[ShaderType::Default];
    shader.bind();
    for (const DrawList::ModelEntry& modelEntry : drawList.models) {
        setUniforms(shader, frame, modelEntry.modelMatrix, modelEntry.normalMatrix);
        modelEntry.model->Draw(shader);
    }
}

void Renderer::drawPrimitives(const FrameData& frame) {
    Shader& shader = shaders[ShaderType::Primitive];
    shader.bind();
    for (const DrawList::PrimitiveEntry& primitiveEntry : drawList.primitives) {
        setUniforms(shader, frame, primitiveEntry.modelMatrix, primitiveEntry.normalMatrix);
        primitiveEntry.primitive->Draw(shader);
    }
}

void Renderer::renderFrame(const Camera& camera, float time, float deltaTime) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    FrameData frame{
        glm::perspective(glm::radians(camera.getFov()), static_cast<float>(width) / height, 0.1f, 100.0f),
        camera,
        time,
        deltaTime
    };

    drawPrimitives(frame);
    drawModels(frame);
	drawList.clear();
}

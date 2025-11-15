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
    for (auto& [type, shader] : shaders) {
        shader.clear();
    }
	activeShader = nullptr;
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
    glLinkProgram(shaders[ShaderType::Default].id);

	vert = std::filesystem::path(SHADER_DIR) / "primitive.vert";
	frag = std::filesystem::path(SHADER_DIR) / "primitive.frag";
	shaders[ShaderType::Primitive] = Shader(vert.string().c_str(), frag.string().c_str());
	glLinkProgram(shaders[ShaderType::Primitive].id);

    useShader(ShaderType::Default);
}

void Renderer::useShader(ShaderType type) {
    activeShader = &shaders[type];  
    activeShader->bind();
}

void Renderer::useShader(Shader& shader) {
	activeShader = &shader;
	activeShader->bind();
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

void Renderer::addPrimitive(Primitive&& primitive) {
    primitives.push_back(std::move(primitive));
}

void Renderer::updateShaderLights() {
    if (hasDirectionalLight) {
        activeShader->setDirectionalLight(dirLight);
    }

    for (std::size_t i = 0; i < pointLights.size(); i++) {
        activeShader->setPointLight(pointLights[i], i);
	}

    for (std::size_t i = 0; i < spotlights.size(); i++) {
        activeShader->setSpotlight(spotlights[i], i);
	}

    activeShader->setuVec3("lightCount", glm::uvec3(
        static_cast<unsigned int>(hasDirectionalLight),
        pointLights.size(),
        spotlights.size()
    ));
}

// assumes shader is already bound
void Renderer::setUniforms(const FrameData& frame, const glm::mat4& model) {
    activeShader->setMat4("u_proj", frame.projection);
    activeShader->setMat4("u_view", frame.camera.getWorldToViewMatrix());
    activeShader->setMat4("u_model", model);
	activeShader->setMat4("u_normalMatrix", glm::transpose(glm::inverse(model)));
    activeShader->setVec3("u_viewPos", frame.camera.getPosition());
    activeShader->setFloat("u_time", frame.time);
    updateShaderLights();
}

void Renderer::submitModel(const Model& model, const glm::mat4& transform) {
	drawList.models.push_back({ &model, transform });
}

void Renderer::submitPrimitive(const Primitive& primitive, const glm::mat4& transform) {
	drawList.primitives.push_back({ &primitive, transform });
}

void Renderer::drawModels(const FrameData& frame) {
    useShader(ShaderType::Default);
    for (const DrawList::ModelEntry& modelEntry : drawList.models) {
        setUniforms(frame, modelEntry.transform);
        modelEntry.model->Draw(shaders[ShaderType::Default]);
    }
}

void Renderer::drawPrimitives(const FrameData& frame) {
    useShader(ShaderType::Primitive);
    for (const DrawList::PrimitiveEntry& primitiveEntry : drawList.primitives) {
        setUniforms(frame, primitiveEntry.transform);
        primitiveEntry.primitive->Draw(shaders[ShaderType::Primitive]);
    }
}

void Renderer::clearDrawList() {
	drawList.clear();
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
}

#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <map>
#include "shader.h"
#include "camera.h"
#include "lightSource.h"
#include "../geometry/model.h"
#include "../geometry/primitive.h"

enum class ShaderType {
    Default,
    Primitive
};

struct FrameData {
    glm::mat4 projection;
    const Camera& camera;
    float time;
    float deltaTime;
};

class Renderer {
public:
    Renderer(int width, int height);
    ~Renderer();

    void init();
    void renderFrame(const Camera& camera, float time, float deltaTime);

    void addLight(const LightSource& light);
    void addModel(const std::string& name);
    void addPrimitive(Primitive&& primitive);
	void useShader(ShaderType type);
    void useShader(Shader& shader);

private:
    int width, height;
	bool hasDirectionalLight = false;
    Shader* activeShader;
    LightSource dirLight;
    std::vector<LightSource> pointLights;
    std::vector<LightSource> spotlights;
    std::vector<Model> models;
    std::vector<Primitive> primitives;
	std::map<ShaderType, Shader> shaders;
    void setupShaders();
    void updateShaderLights();
	void setUniforms(const Camera& camera, glm::mat4 projection, float time, float deltaTime);
    void drawModels(const FrameData& frame);
    void drawPrimitives(const FrameData& frame);
};
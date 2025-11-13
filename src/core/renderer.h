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

struct DrawList {
    struct ModelEntry {
        const Model* model;
        glm::mat4 transform;
    };

    struct PrimitiveEntry {
        const Primitive* primitive;
        glm::mat4 transform;
    };

    struct LightEntry {
        LightSource light;
        glm::mat4 transform;
    };

    std::vector<ModelEntry> models;
    std::vector<PrimitiveEntry> primitives;
    std::vector<LightEntry> lights;

    void clear() {
        models.clear();
        primitives.clear();
        lights.clear();
    }
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
	void submitModel(const Model& model, const glm::mat4& transform);
	void submitPrimitive(const Primitive& primitive, const glm::mat4& transform);
    void clearDrawList();
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
	DrawList drawList;
    void setupShaders();
    void updateShaderLights();
	void setUniforms(const FrameData& frame, const glm::mat4& pos = glm::mat4(1.0f));
    void drawModels(const FrameData& frame);
    void drawPrimitives(const FrameData& frame);
};
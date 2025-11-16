#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <map>
#include "shader.h"
#include "camera.h"
#include "../geometry/model.h"
#include "../geometry/primitive.h"
#include "lightData.h"

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

    std::vector<ModelEntry> models;
    std::vector<PrimitiveEntry> primitives;
    std::vector<PointLight> pointLights;
    std::vector<Spotlight> spotlights;
	DirectionalLight directionalLight;
	bool hasDirectionalLight = false;

    void clear() {
        models.clear();
        primitives.clear();
		pointLights.clear();
		spotlights.clear();
        hasDirectionalLight = false;
    }
};

class Renderer {
public:
    Renderer(int width, int height);
    ~Renderer();

    void init();
    void renderFrame(const Camera& camera, float time, float deltaTime);

    void addModel(const std::string& name);
    void addPrimitive(Primitive&& primitive);
	void useShader(ShaderType type);
    void useShader(Shader& shader);
	void submit(const Model& model, const glm::mat4& transform); // model submission
	void submit(const Primitive& primitive, const glm::mat4& transform); // primitive submission
	void submit(const DirectionalLight& l); // directional light submission
	void submit(const PointLight& l); // point light submission
	void submit(const Spotlight& l); // spotlight submission
    void clearDrawList();
private:
    int width, height;
    Shader* activeShader;
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
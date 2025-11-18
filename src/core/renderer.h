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
        glm::mat4 modelMatrix;
		glm::mat4 normalMatrix;
    };

    struct PrimitiveEntry {
        const Primitive* primitive;
        glm::mat4 modelMatrix;
        glm::mat4 normalMatrix;
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
	void submit(const Model& model, const glm::mat4& transform, const glm::mat4& normalMatrix); // model submission
	void submit(const Primitive& primitive, const glm::mat4& transform, const glm::mat4& normalMatrix); // primitive submission
	void submit(const DirectionalLight& l); // directional light submission
	void submit(const PointLight& l); // point light submission
	void submit(const Spotlight& l); // spotlight submission
private:
    int width, height;
    std::vector<Model> models;
    std::vector<Primitive> primitives;
	std::map<ShaderType, Shader> shaders;
	DrawList drawList;
    void setupShaders();
    void updateShaderLights(Shader& shader);
	void setUniforms(Shader& shader, const FrameData& frame, const glm::mat4& model = glm::mat4(1.0f), 
        const glm::mat4& normal = glm::mat4(1.0f));
    void drawModels(const FrameData& frame);
    void drawPrimitives(const FrameData& frame);
};
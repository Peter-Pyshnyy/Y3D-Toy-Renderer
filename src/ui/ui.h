#pragma once

#include "imgui.h"
#include "../core/renderer.h"
#include "../core/scene/sceneNode.h"
#include "../core/camera.h"
#include <GLFW/glfw3.h>

class UI {
public:
    UI(GLFWwindow* window);
    ~UI();

    void begin();
    void end();
    void createDockSpace();
    void createViewportWindow(Camera& camera, ImTextureID texture);
	void createHierarchyWindow(SceneNode& scene);
	void createPropertiesWindow();
    void renderGizmo(Camera& camera, ImVec2 viewportSize, ImVec2 finalSize);

	SceneNode* selectedNode = nullptr;
private:
    void recursiveHierarchy(SceneNode& node);
    GLFWwindow* window;
};

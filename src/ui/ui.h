#pragma once
#include "../core/renderer.h"
#include "../core/scene/sceneNode.h"
#include <GLFW/glfw3.h>

class UI {
public:
    UI(GLFWwindow* window);
    ~UI();

    void begin();
    void end();
    void createDockSpace();
    void createViewportWindow(Renderer& renderer);
	void createHierarchyWindow(SceneNode& scene);

	SceneNode* selectedNode = nullptr;
private:
    void recursiveHierarchy(SceneNode& node);
    GLFWwindow* window;
};

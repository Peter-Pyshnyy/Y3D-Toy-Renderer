#pragma once
#include "../core/renderer.h"
#include <GLFW/glfw3.h>

class UI {
public:
    UI(GLFWwindow* window);
    ~UI();

    void begin();
    void end();
    void createDockSpace();
    void createViewportWindow(Renderer& renderer);
	void createHierarchyWindow();

private:
    GLFWwindow* window;
};

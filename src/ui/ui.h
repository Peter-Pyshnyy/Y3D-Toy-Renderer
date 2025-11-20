#pragma once
#include <GLFW/glfw3.h>

class UI {
public:
    UI(GLFWwindow* window);
    ~UI();

    void begin();
    void end();

private:
    GLFWwindow* window;
};

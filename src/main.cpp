#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include "utils/openglErrorReporting.h"
#include "core/renderer.h"
#include "core/camera.h"
#include "core/lightSource.h"

GLuint width = 1280;
GLuint height = 720;
GLuint numIndices = 0;
Camera camera;
Renderer renderer(width, height);

float lastFrame = 0.0f;

#pragma region callbacks
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	camera.mouseUpdate(glm::vec2(xpos, ypos));
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, true);
		}
		if (key == GLFW_KEY_W) {
			camera.setMoving(camera.FORWARD, true);
		}
		if (key == GLFW_KEY_A) {
			camera.setMoving(camera.LEFT, true);
		}
		if (key == GLFW_KEY_S) {
			camera.setMoving(camera.BACKWARD, true);
		}
		if (key == GLFW_KEY_D) {
			camera.setMoving(camera.RIGHT, true);
		}
		if (key == GLFW_KEY_Q) {
			camera.setMoving(camera.DOWN, true);
		}
		if (key == GLFW_KEY_E) {
			camera.setMoving(camera.UP, true);
		}
		if (key == GLFW_KEY_LEFT_SHIFT) {
			camera.setAcceleration(true);
		}
	}

	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_W) {
			camera.setMoving(camera.FORWARD, false);
		}
		if (key == GLFW_KEY_A) {
			camera.setMoving(camera.LEFT, false);
		}
		if (key == GLFW_KEY_S) {
			camera.setMoving(camera.BACKWARD, false);
		}
		if (key == GLFW_KEY_D) {
			camera.setMoving(camera.RIGHT, false);
		}
		if (key == GLFW_KEY_Q) {
			camera.setMoving(camera.DOWN, false);
		}
		if (key == GLFW_KEY_E) {
			camera.setMoving(camera.UP, false);
		}
		if (key == GLFW_KEY_LEFT_SHIFT) {
			camera.setAcceleration(false);
		}
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.zoom(yoffset);
}
#pragma endregion

int main() {
#pragma region window-setup
	if (!glfwInit()) {
		std::cout << "GLFW Init Error!" << "\n";
		return -1;
	}
	GLFWwindow* window = glfwCreateWindow(width, height, "window", NULL, NULL);
	if (!window) {
		std::cout << "Window Error!" << "\n";
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		return 1;
	}
	enableReportGlErrors();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);
#pragma endregion

#pragma region renderer
	renderer.init();
	
	// light setup
	LightSource light;
	light.constructAsDirectionalLight(glm::vec3(1.0, -1.0, -1.0));
	renderer.addLight(light);

	// model loading
	renderer.addModel("backpack");
#pragma endregion

	// render loop
	while (!glfwWindowShouldClose(window)) {
		int w = 0, h = 0;
		glfwGetWindowSize(window, &w, &h);

		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		camera.move(deltaTime);
		renderer.renderFrame(camera, glfwGetTime(), deltaTime);
		lastFrame = currentFrame;
		glfwSwapBuffers(window); //presents the contents of an internel buffer to the screen
		glfwPollEvents(); //window event hanlder
	}

	// cleanup
	glfwDestroyWindow(window);
	glfwTerminate();
}
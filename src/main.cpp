#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <filesystem>
#include "openglErrorReporting.h"
#include "shader.h"
#include "Camera.h"
#include "primitives/model.h"
#include "paths.h"

GLuint numIndices = 0;
Camera camera;
float fov = 40.0f;
float lastFrame = 0.0f;

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
	fov -= (float)yoffset;
	if (fov < 1.0f) {
		fov = 1.0f;
	}
	if (fov > 45.0f) {
		fov = 45.0f;
	}
}

int main() {
#pragma region setup
	if (!glfwInit()) {
		std::cout << "GLFW Init Error!" << "\n";
		return -1;
	}
	GLFWwindow* window = glfwCreateWindow(640, 480, "window", NULL, NULL);
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
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
#pragma endregion

#pragma region vao
	GLuint vao; //vertex array object
	glGenVertexArrays(1, &vao); //generates a vertex array object
	glBindVertexArray(vao); //binds the vertex array object to the current context
#pragma endregion

	glBindVertexArray(0); //unbinds the vertex array object

#pragma region shader loading
	//load the shaders	
	Shader shader;
	std::filesystem::path vertPath = std::filesystem::path(SHADER_DIR) / "shader.vert";
	std::filesystem::path fragPath = std::filesystem::path(SHADER_DIR) / "shader.frag";
	shader.loadShaderProgramFromFile(vertPath.string().c_str(), fragPath.string().c_str());
	shader.bind(); //binds the shader program to the current context
	//-----------------------------------
	glLinkProgram(shader.id);

	GLint success;
	glGetProgramiv(shader.id, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(shader.id, 512, NULL, infoLog);
		std::cout << "Shader link error:\n" << infoLog << "\n";
	}

	GLint count = 0;
	glGetProgramiv(shader.id, GL_ACTIVE_UNIFORMS, &count);
	std::cout << "Active uniforms: " << count << "\n";
#pragma endregion

	Model ourModel(std::filesystem::path("../resources/models/backpack.obj").string());
	// render loop
	while (!glfwWindowShouldClose(window)) {
		int w = 0, h = 0;
		glfwGetWindowSize(window, &w, &h);
		glViewport(0, 0, w, h); //sets the viewport to the size of the window
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(vao); //binds the vertex array object
		shader.bind(); //binds the shader program to the current context
		glm::mat4 proj = glm::perspective(glm::radians(fov), static_cast<float>(w) / static_cast<float>(h), 0.1f, 100.0f);

		shader.setFloat("u_time", (float)glfwGetTime());
		shader.setMat4("u_proj", proj);
		shader.setMat4("u_view", camera.getWorldToViewMatrix());
		shader.setVec3("u_viewPos", camera.getPosition());
		shader.setVec3("u_lightPos", glm::vec3(1.0f, 1.0f, 5.0f)); //sets the light position uniform variable

		//glDrawElementsInstanced(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0, modelTransforms.size()); //draws the triangles
		ourModel.Draw(shader);

		float currentFrame = glfwGetTime();
		camera.move(currentFrame - lastFrame);
		lastFrame = currentFrame;
		glfwSwapBuffers(window); //presents the contents of an internel buffer to the screen
		glfwPollEvents(); //window event hanlder
	}

	// cleanup
	shader.clear();
	glfwDestroyWindow(window);
	glfwTerminate();
}
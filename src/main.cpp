#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <memory>
#include <filesystem>
#include "utils/paths.h"
#include "utils/openglErrorReporting.h"
#include "core/renderer.h"
#include "core/scene/scene.h"
#include "core/scene/sceneNode.h" //raplace later with empty node
#include "core/scene/nodes/modelNode.h"
#include "core/scene/nodes/primitiveNode.h"
#include "core/scene/nodes/directionalLightNode.h"
#include "core/scene/nodes/pointLightNode.h"
#include "core/scene/nodes/spotlightNode.h"
#include "core/scene/nodes/emptyNode.h"
#include "geometry/primitive.h"
#include "core/camera.h"

#include "ui/ui.h"

GLuint width = 1280;
GLuint height = 720;
const GLfloat helper = 1.031375 * 0.75; // there is surely a better way to do it, but this roughly derives viewport aspect ratio from window aspect ratio (given VP is 0.75 of dockspace);   
Camera camera((static_cast<float>(width) / height) * helper);
Renderer renderer;
Scene scene;

bool cameraMode = false;
float lastFrame = 0.0f;

#pragma region callbacks
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (!cameraMode) return;
	camera.mouseUpdate(glm::vec2(xpos, ypos));
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		cameraMode = true;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		cameraMode = false;
	}
}

void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (!cameraMode) return;
	camera.zoom(yoffset);
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
			if (cameraMode) {
				camera.setMoving(camera.DOWN, true);
			}
			else {
				UI* ui = static_cast<UI*>(glfwGetWindowUserPointer(window));
				ui->gizmoOperation = 7; // translate
			}
		}
		if (key == GLFW_KEY_E) {
			if (cameraMode) {
				camera.setMoving(camera.UP, true);
			}
			else {
				UI* ui = static_cast<UI*>(glfwGetWindowUserPointer(window));
				ui->gizmoOperation = 56; // rotate
			}
		}
		if (key == GLFW_KEY_R) {
			if (cameraMode) return;
			UI* ui = static_cast<UI*>(glfwGetWindowUserPointer(window));
			ui->gizmoOperation = 128; // scaleX
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
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);

	UI ui(window);
	glfwSetWindowUserPointer(window, &ui); // lets us access the ui object using static_cast<UI*>(glfwGetWindowUserPointer(window));
#pragma endregion

#pragma region renderer
	renderer.init();
	renderer.createFramebuffer(width, height);

	std::filesystem::path path = std::filesystem::path(MODEL_DIR) / std::filesystem::path("LibertyStatue/LibertStatue.obj");
	std::shared_ptr<Model> backpackModel = std::make_shared<Model>(path.string());
	std::unique_ptr<ModelNode> modelNode = std::make_unique<ModelNode>("model", backpackModel);

	modelNode->translate(glm::vec3(0.0f, -1.0f, 0.0f));
	modelNode->scale(3.75f);

	std::unique_ptr<PointLightNode> pointLight = std::make_unique<PointLightNode>("point light");
	pointLight->translate(glm::vec3(-0.225f, 1.4f, 0.25f));
	modelNode->addChild(std::move(pointLight));
	pointLight = std::make_unique<PointLightNode>("point light2");
	pointLight->translate(glm::vec3(-0.225f, 1.4f, 0.25f));
	pointLight->properties.linear = 0.7f;
	pointLight->properties.quadratic = 1.8f;
	modelNode->addChild(std::move(pointLight));

	std::shared_ptr<Primitive> superCubePrim = std::make_shared<Primitive>(PrimitiveType::Cube, 1.0f, glm::vec3(255.f, 255.f, 255.f) / 255.0f);
	std::unique_ptr<PrimitiveNode> superCube = std::make_unique<PrimitiveNode>("cube", superCubePrim);
	float halfPi = 3.14159265f / 2.0;
	for (int i = 0; i < 4; i++) {
		std::shared_ptr<Primitive> cubePrim = std::make_shared<Primitive>(PrimitiveType::Cube, 1.0f, glm::vec3(255.f, 10.f, 10.f) / 255.0f);
		std::unique_ptr<PrimitiveNode> cube1 = std::make_unique<PrimitiveNode>("cube", cubePrim);
		cube1->scale(0.5f);
		cube1->translate(glm::vec3(2.0f * round(cos(halfPi * i)), 0.0f, 2.0f * round(sin(halfPi * i))));
		superCube->addChild(std::move(cube1));
	}

	for (int i = 0; i < 2; i++) {
		std::shared_ptr<Primitive> cubePrim = std::make_shared<Primitive>(PrimitiveType::Cube, 1.0f, glm::vec3(255.f, 10.f, 10.f) / 255.0f);
		std::unique_ptr<PrimitiveNode> cube1 = std::make_unique<PrimitiveNode>("cube", cubePrim);
		cube1->scale(0.5f);
		cube1->translate(glm::vec3(0.0f, 2.0f * (i * 2.0f - 1.0f), 0.0f));
		superCube->addChild(std::move(cube1));
	}

	superCube->translate(glm::vec3(-5.0f, 2.5f, -2.0f));
	superCube->scale(0.5f);

	std::shared_ptr<Primitive> plane = std::make_shared<Primitive>(PrimitiveType::Plane, 10.0f, glm::vec3(0.2f, 1.0f, 0.35f));
	std::unique_ptr<PrimitiveNode> floorNode = std::make_unique<PrimitiveNode>("plane", plane);
	floorNode->translate(glm::vec3(0.0f, -1.0f, 0.0f));

	std::unique_ptr<SceneNode> lightHolder = std::make_unique<SceneNode>("light holder");

	for(int i = 0; i < 5; i++) {
		std::unique_ptr<SceneNode> emptyNode = std::make_unique<SceneNode>("empty");

		std::unique_ptr<SpotlightNode> spotlightNode = std::make_unique<SpotlightNode>("spotlight");
		spotlightNode->rotate(glm::vec3(-90.0f, 0.0f, 0.0f));
		spotlightNode->translate(glm::vec3(cos(i) * 2.0f, 0.5f + i * 0.2f, sin(i) * 2.0f));

		emptyNode->addChild(std::move(spotlightNode));
		emptyNode->translate(glm::vec3(i * 0.3));
		lightHolder->addChild(std::move(emptyNode));
	}

	lightHolder->translate(glm::vec3(5.0f, 0.0f, 0.0f));

	std::unique_ptr<DirectionalLightNode> dirLightNode = std::make_unique<DirectionalLightNode>();

	scene.getRoot()->addChild(std::move(floorNode));
	scene.getRoot()->addChild(std::move(lightHolder));
	scene.getRoot()->addChild(std::move(modelNode));
	scene.getRoot()->addChild(std::move(superCube));
	scene.getRoot()->addChild(std::move(dirLightNode));
	scene.submit(renderer);
#pragma endregion

	// render loop
	while (!glfwWindowShouldClose(window)) {
		int w = 0, h = 0;
		glfwGetWindowSize(window, &w, &h);

		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;

		scene.getRoot()->getChild(3)->rotate(glm::vec3(20.0f * deltaTime, -30.0f * deltaTime, 0.0f));

		scene.submit(renderer);
		
		if (cameraMode) {
			camera.move(deltaTime);
		}

		renderer.renderFrame(camera, glfwGetTime(), deltaTime);
		lastFrame = currentFrame;

		ui.begin();
		ui.createDockSpace();
		ui.createHierarchyWindow(*scene.getRoot());
		ui.createViewportWindow(camera, renderer.colorTex);
		ui.createPropertiesWindow();
		ui.end();

		glfwSwapBuffers(window); //presents the contents of an internal buffer to the screen
		glfwPollEvents(); //window event hanlder
	}

	// cleanup
	glfwDestroyWindow(window);
	glfwTerminate();
}
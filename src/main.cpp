#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <imgui.h>
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
GLuint numIndices = 0;
Camera camera;
Renderer renderer(width, height);
Scene scene;

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

	UI ui(window);
#pragma endregion

#pragma region renderer
	renderer.init();

	std::filesystem::path path = std::filesystem::path(MODEL_DIR) / std::filesystem::path("BrandenburgGate/BrandenburgGate.obj");
	std::shared_ptr<Model> backpackModel = std::make_shared<Model>(path.string());
	std::unique_ptr<ModelNode> modelNode = std::make_unique<ModelNode>("model", backpackModel);

	/*std::unique_ptr<ModelNode> modelNode2 = std::make_unique<ModelNode>("test2", backpackModel);
	std::unique_ptr<PrimitiveNode> cubeNode = std::make_unique<PrimitiveNode>("cube");
	std::unique_ptr<SpotlightNode> pl = std::make_unique<SpotlightNode>("spotlight");
	std::unique_ptr<DirectionalLightNode> dl = std::make_unique<DirectionalLightNode>();
	std::unique_ptr<SceneNode> emptyNode = std::make_unique<SceneNode>("empty");

	pl->rotate(glm::vec3(0.0f, 180.0f, 0.0f));

	cubeNode->translate(glm::vec3(0.0f, 0.0f, -4.0f));
	cubeNode->scale(0.3f);
	pl->translate(glm::vec3(1.0f, 0.0f, 1.0f));

	cubeNode->addChild(std::move(pl));

	emptyNode->addChild(std::move(cubeNode));
	modelNode->addChild(std::move(emptyNode));
	dl->rotate(glm::vec3(45.0f, 0.0f, 0.0f));
	modelNode->addChild(std::move(dl));

	scene.getRoot()->addChild(std::move(modelNode));

	modelNode2->translate(glm::vec3(1.0f, 0.0f, 0.0f));
	modelNode2->scale(0.25f);

	scene.getRoot()->addChild(std::move(modelNode2));*/

	std::shared_ptr<Primitive> plane = std::make_shared<Primitive>(PrimitiveType::Plane, 10.0f);
	std::unique_ptr<PrimitiveNode> floorNode = std::make_unique<PrimitiveNode>("plane", plane);
	floorNode->translate(glm::vec3(0.0f, -1.0f, 0.0f));

	modelNode->rotate(glm::vec3(0.0f, 0.0f, 0.0f));
	modelNode->scale(1.75f);

	std::unique_ptr<SceneNode> lightHolder = std::make_unique<SceneNode>("light holder");

	for(int i = 0; i < 5; i++) {
		std::unique_ptr<SceneNode> emptyNode = std::make_unique<SceneNode>("empty");

		std::unique_ptr<SpotlightNode> spotlightNode = std::make_unique<SpotlightNode>("spotlight");
		spotlightNode->rotate(glm::vec3(-90.0f, 0.0f, 0.0f));
		spotlightNode->translate(glm::vec3(i*1.25, 1.0f + i * 0.25f, i * 0.6f));

		emptyNode->addChild(std::move(spotlightNode));
		emptyNode->translate(glm::vec3(i * 0.3));
		lightHolder->addChild(std::move(emptyNode));
	}

	std::unique_ptr<DirectionalLightNode> dirLightNode = std::make_unique<DirectionalLightNode>();

	scene.getRoot()->addChild(std::move(floorNode));
	scene.getRoot()->addChild(std::move(lightHolder));
	scene.getRoot()->addChild(std::move(modelNode));
	scene.getRoot()->addChild(std::move(dirLightNode));
	scene.submit(renderer);
#pragma endregion

	// render loop
	while (!glfwWindowShouldClose(window)) {
		int w = 0, h = 0;
		glfwGetWindowSize(window, &w, &h);

		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;

		//scene.getRoot()->getChild(0)->getChild(0)->rotate(glm::vec3(0.0f, 15.0f * deltaTime, 0.0f));
		scene.getRoot()->getChild(1)->rotate(glm::vec3(0.0f, -30.0f * deltaTime, 0.0f));

		for (int i = 0; i < 5; i++) {
			scene.getRoot()->getChild(1)->getChild(i)->rotate(glm::vec3(0.0f, i * 0.5 * 60.0f * deltaTime, 0.0f));
		}

		scene.getRoot()->getChild(2)->rotate(glm::vec3(0.0f, -10.0f * deltaTime, 0.0f));

		scene.submit(renderer);
		
		camera.move(deltaTime);
		renderer.renderFrame(camera, glfwGetTime(), deltaTime);
		lastFrame = currentFrame;

		/*ui.begin();

		ImGui::Begin("Test");
		ImGui::Text("Hello hello, I'm Peter");
		ImGui::End();

		ui.end();*/

		glfwSwapBuffers(window); //presents the contents of an internel buffer to the screen
		glfwPollEvents(); //window event hanlder
	}

	// cleanup
	glfwDestroyWindow(window);
	glfwTerminate();
}
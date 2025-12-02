#include "UI.h"

#include <imgui_internal.h>
#include "imgui_guizmo.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

float uvOffset = 0.5;

UI::UI(GLFWwindow* window) : window(window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // enable docking

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

UI::~UI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UI::begin()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
	ImGuizmo::BeginFrame();

    // create a docking space over the entire viewport
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(),
        ImGuiDockNodeFlags_PassthruCentralNode); 
}

void UI::end()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::createDockSpace() {
	// sets the window's position to top-left of main window and size to full window size
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

	// attach window to main viewport (maybe for future multi-viewport setups?)
	ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);

	ImGuiWindowFlags flags =
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus;

	ImGui::Begin("RootDockspace", nullptr, flags);
	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockspace_id);

	static bool first = true;
	if (first) {
		first = false;

		ImGui::DockBuilderRemoveNode(dockspace_id);
		ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

		ImGuiID left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.25f, nullptr, &dockspace_id);
		ImGuiID center = dockspace_id;
		ImGuiID leftBot = ImGui::DockBuilderSplitNode(left, ImGuiDir_Down, 0.40f, nullptr, &left);

		ImGui::DockBuilderDockWindow("Hierarchy", left);
		ImGui::DockBuilderDockWindow("Viewport", center);
		ImGui::DockBuilderDockWindow("Properties", leftBot);

		ImGui::DockBuilderFinish(dockspace_id);
	}
	ImGui::End();
}

void UI::createViewportWindow(Camera& camera, ImTextureID texture) {
	ImGui::Begin("Viewport");
	ImVec2 viewportSize = ImGui::GetContentRegionAvail();
	float texAspect = camera.ASPECT_RATIO;

	ImVec2 uv0, uv1;

	float viewAspect = viewportSize.x / viewportSize.y;
	if (viewAspect > texAspect) {
		// viewport wider -> crop horizontally
		float offset = (texAspect / viewAspect) / 2.0;
		uv0 = ImVec2(0, 0.5 + offset);
		uv1 = ImVec2(1, 0.5 - offset);
	}
	else {
		// viewport taller -> crop vertically
		float offset = (viewAspect / texAspect) / 2.0;
		uv0 = ImVec2(0.5 - offset, 1.0f);
		uv1 = ImVec2(0.5f + offset, 0.0f);
	}

	//ImGui::Image(texture, viewportSize, uv0, uv1); // remove for gizmo testing
	ImGui::Image(texture, viewportSize, ImVec2(0, 1), ImVec2(1, 0));


	renderGizmo(camera);

	ImGui::End();
}

void UI::createHierarchyWindow(SceneNode& root) {
	ImGui::Begin("Hierarchy");
	
	ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
	if (ImGui::TreeNodeEx("root", flag)) {
		for (auto &child : root.children) {
			recursiveHierarchy(*child);
		}
		ImGui::TreePop();
	}
	ImGui::End();
}

void UI::recursiveHierarchy(SceneNode& node) {
	ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth;
	if (node.children.empty()) {
		flag |= ImGuiTreeNodeFlags_Leaf;
	}
	else {
		flag |= ImGuiTreeNodeFlags_OpenOnArrow;
	}
	if (&node == selectedNode) {
		flag |= ImGuiTreeNodeFlags_Selected;
	}

	// (void*)&node to ensure unique ID (its adress) even with same names
	// note: TreeNodeEx returns whether node is expanded this frame
	bool opened = ImGui::TreeNodeEx((void*)&node, flag, node.name.c_str());

	if (ImGui::IsItemClicked()) {
		selectedNode = &node;
	}

	if (opened) {
		for (auto& child : node.children) {
			recursiveHierarchy(*child);
		}
		ImGui::TreePop();
	}
}

void UI::createPropertiesWindow() {
	ImGui::Begin("Properties");
	if (selectedNode) {
		glm::vec3 pos = selectedNode->transform.position;
		glm::vec3 rot = selectedNode->transform.rotation;
		float scl = selectedNode->transform.scale;

		ImGui::Text("Name: %s", selectedNode->name.c_str());
		ImGui::Separator();
		ImGui::Text("Transform:");
		if (ImGui::DragFloat3("Position", &pos.x, 0.1f)) {
			selectedNode->translate(pos - selectedNode->transform.position);
		}
		if (ImGui::DragFloat3("Rotation", &rot.x, 1.0f)) {
			selectedNode->rotate(rot - selectedNode->transform.rotation);
		}
		if (ImGui::DragFloat("Scale", &scl, 0.1f, 0.01f, 100.0f)) {
			selectedNode->scale(scl / selectedNode->transform.scale);
		};
	}
	ImGui::End();
}

void UI::renderGizmo(Camera& camera) {
	ImVec2 winPos = ImGui::GetWindowPos();
	ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
	ImVec2 contentMax = ImGui::GetWindowContentRegionMax();

	ImVec2 pos(winPos.x + contentMin.x, winPos.y + contentMin.y);
	ImVec2 size(contentMax.x - contentMin.x, contentMax.y - contentMin.y);

	ImGuizmo::SetRect(pos.x, pos.y, size.x, size.y);
	ImGuizmo::SetOrthographic(false);
	ImGuizmo::SetDrawlist();



	if (selectedNode) {
		ImGuizmo::Enable(true);
		glm::mat4 model = selectedNode->getWorldTransform();
		glm::mat4 view = camera.getWorldToViewMatrix();
		glm::mat4 proj = camera.getProjectionMatrix();

		// Convert to float pointer (ImGuizmo uses row-major float arrays)
		float viewArr[16], projArr[16], modelArr[16];
		memcpy(viewArr, glm::value_ptr(view), sizeof(viewArr));
		memcpy(projArr, glm::value_ptr(proj), sizeof(projArr));
		memcpy(modelArr, glm::value_ptr(model), sizeof(modelArr));

		static ImGuizmo::OPERATION currentOp = ImGuizmo::TRANSLATE;
		static ImGuizmo::MODE currentMode = ImGuizmo::WORLD;

		ImGuizmo::Manipulate(viewArr, projArr,
			currentOp, currentMode,
			modelArr, nullptr, nullptr);

		//std::cout << ImGuizmo::IsViewManipulateHovered() << "\n";

		if (ImGuizmo::IsUsing()) {
			glm::vec3 pos, rot, scl;

			ImGuizmo::DecomposeMatrixToComponents(modelArr,
				glm::value_ptr(pos),
				glm::value_ptr(rot),
				glm::value_ptr(scl));

			if (pos != selectedNode->transform.position) {
				selectedNode->translate(pos - selectedNode->transform.position);
			}
			if (rot != selectedNode->transform.rotation) {
				selectedNode->rotate(rot - selectedNode->transform.rotation);
			}
			if (scl.x != selectedNode->transform.scale) {
				selectedNode->scale(scl.x / selectedNode->transform.scale);
			}
		}
	}
}
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

	float imageAspect = camera.ASPECT_RATIO;
	float viewportAspect = viewportSize.x / viewportSize.y;

	ImVec2 finalSize;
	if (viewportAspect > imageAspect) {
		finalSize.y = viewportSize.y;
		finalSize.x = finalSize.y * imageAspect;
	}
	else {
		finalSize.x = viewportSize.x;
		finalSize.y = finalSize.x / imageAspect;
	}

	// center the image in the viewport
	ImVec2 cursorPos = ImGui::GetCursorPos();
	cursorPos.x += (viewportSize.x - finalSize.x) * 0.5f;
	cursorPos.y += (viewportSize.y - finalSize.y) * 0.5f;
	ImGui::SetCursorPos(cursorPos);

	ImGui::Image(texture, finalSize, ImVec2(0, 1), ImVec2(1, 0));

	// render gizmo on top of viewport, adjusting for cropping
	renderGizmo(camera, viewportSize, finalSize);


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
			std::cout << rot.x - selectedNode->transform.rotation.x << ", "
				<< rot.y - selectedNode->transform.rotation.y << ", "
				<< rot.z - selectedNode->transform.rotation.z << "\n";
			selectedNode->rotate(rot - selectedNode->transform.rotation);
		}
		if (ImGui::DragFloat("Scale", &scl, 0.1f, 0.01f, 100.0f)) {
			selectedNode->scale(scl / selectedNode->transform.scale);
		};
	}
	ImGui::End();
}

void UI::renderGizmo(Camera& camera, ImVec2 viewportSize, ImVec2 finalSize) {

	// --------- adjusting gizmo to viewport cropping ---------
	ImVec2 windowPos = ImGui::GetWindowPos();
	ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
	ImVec2 contentPos(windowPos.x + contentMin.x, windowPos.y + contentMin.y);
	ImVec2 imageOffset = ImVec2(
		(viewportSize.x - finalSize.x) * 0.5f,
		(viewportSize.y - finalSize.y) * 0.5f
	);

	ImVec2 imageTopLeft = ImVec2(
		contentPos.x + imageOffset.x,
		contentPos.y + imageOffset.y
	);
	ImGuizmo::SetRect(imageTopLeft.x, imageTopLeft.y, finalSize.x, finalSize.y);
	// ---------------------------------------------------------

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

		// --------- styles and operations ---------
		ImGuizmo::OPERATION currentOp = ImGuizmo::OPERATION(gizmoOperation);
		static ImGuizmo::MODE currentMode = ImGuizmo::WORLD;
		ImGuizmo::AllowAxisFlip(false);
		ImGuizmo::SetGizmoSizeClipSpace(0.15f);

		auto& style = ImGuizmo::GetStyle();
		style.TranslationLineThickness = 7.0f;
		style.TranslationLineArrowSize = 10.0f;
		style.RotationLineThickness = 4.0f;
		style.ScaleLineThickness = 7.0;
		style.ScaleLineCircleSize = 10.0f;
		// -----------------------------------------

		ImGuizmo::Manipulate(viewArr, projArr,
			currentOp, currentMode,
			modelArr, nullptr, nullptr); 

		if (ImGuizmo::IsUsing()) {
			// get new local tranform
			glm::mat4 worldNew = glm::make_mat4(modelArr);
			glm::mat4 modelNew = glm::inverse(selectedNode->getParent()->getWorldTransform()) * worldNew;
			// uniform scaling
			glm::vec3 col0 = glm::vec3(modelNew[0]);
			glm::vec3 col1 = glm::vec3(modelNew[1]);
			glm::vec3 col2 = glm::vec3(modelNew[2]);

			float newSx = glm::length(col0);

			// guard against degenerate case
			if (newSx > 0.0f) {
				glm::vec3 col0Norm = glm::normalize(col0);
				glm::vec3 col1Norm = glm::normalize(col1);
				glm::vec3 col2Norm = glm::normalize(col2);

				// glm::scale() would corrupt rotation and translation
				modelNew[0] = glm::vec4(col0Norm * newSx, 0.0f);
				modelNew[1] = glm::vec4(col1Norm * newSx, 0.0f);
				modelNew[2] = glm::vec4(col2Norm * newSx, 0.0f);
			}

			selectedNode->setModelMatrix(modelNew);
		}
	}
}
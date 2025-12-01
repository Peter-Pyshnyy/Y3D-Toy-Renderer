#include "UI.h"

#include "imgui.h"
#include <imgui_internal.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
//#include "../core/renderer.h"

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
		ImGuiID right = dockspace_id;

		ImGui::DockBuilderDockWindow("Hierarchy", left);
		ImGui::DockBuilderDockWindow("Viewport", right);

		ImGui::DockBuilderFinish(dockspace_id);
	}
	ImGui::End();
}

void UI::createViewportWindow(Renderer& renderer) {
	ImGui::Begin("Viewport");
	ImVec2 viewportSize = ImGui::GetContentRegionAvail();

	// if size changed, update framebuffer size
	if ((int)viewportSize.x != renderer.width || (int)viewportSize.y != renderer.height) {
		renderer.createFramebuffer((int)viewportSize.x, (int)viewportSize.y);
	}

	// draw the texture (note the UVs {0,1} to {1,0} flips vertically)
	ImGui::Image((ImTextureID)renderer.colorTex, viewportSize, ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
}

void UI::createHierarchyWindow() {
	ImGui::Begin("Hierarchy");
	ImGui::Text("test");
	ImGui::End();
}

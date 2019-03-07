#include "PhotonBox/core/system/DebugGUI.h"

#include "PhotonBox/core/Display.h"
#include "PhotonBox/component/Camera.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

void DebugGUI::init()
{
	std::cout << "Initializing DebugGUI";
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(Display::getWindow(), true);
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	
	ImGui::StyleColorsDark();

	ImGuizmo::Enable(true);
	std::cout << " - Done\n";
}

void DebugGUI::newFrame()
{
	ImGui_ImplGlfwGL3_NewFrame();
	ImGuizmo::BeginFrame();
}

void DebugGUI::render()
{
	static const float identityMatrix[16] =
	{ 1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f };

	ImGuizmo::DrawGrid(&(Camera::getMainCamera()->getViewMatrix()(0, 0)), &(Camera::getMainCamera()->getProjectionMatrix()(0, 0)), identityMatrix, 100.f);
	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}

void DebugGUI::destroy()
{
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
}

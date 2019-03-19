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

	std::cout << " - Done\n";
}

void DebugGUI::newFrame()
{
	ImGui_ImplGlfwGL3_NewFrame();
}

void DebugGUI::render()
{
	static const float identityMatrix[16] =
	{ 1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f };

	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}

void DebugGUI::destroy()
{
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
}

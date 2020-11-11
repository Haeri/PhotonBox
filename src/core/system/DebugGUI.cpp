#include "PhotonBox/core/system/DebugGUI.h"

#include "../../imgui/imgui_impl_glfw.h"
#include "../../imgui/imgui_impl_opengl3.h"

#include "PhotonBox/core/Display.h"
#include "PhotonBox/component/Camera.h"
#include "PhotonBox/util/Logger.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

void DebugGUI::init(Config::Profile profile)
{
	Logger::infoln("Initializing DebugGUI");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui_ImplGlfw_InitForOpenGL(Display::getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 130");
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	
	ImGui::StyleColorsDark();
}

void DebugGUI::newFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void DebugGUI::render()
{
	static const float identityMatrix[16] =
	{ 1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f };

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void DebugGUI::start()
{
	newFrame();
}

void DebugGUI::reset()
{
	ImGui::EndFrame();
}

void DebugGUI::destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

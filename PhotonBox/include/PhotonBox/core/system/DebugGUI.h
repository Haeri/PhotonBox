#ifndef DEBUG_GUI_H
#define DEBUG_GUI_H

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "imgui/ImGuizmo.h"

#include "PhotonBox/core/System.h"

class DebugGUI : public System
{
public:
	void init() override;
	void start() override {}
	void destroy() override;

	void newFrame();
	void render();
};

#endif // DEBUG_GUI_H

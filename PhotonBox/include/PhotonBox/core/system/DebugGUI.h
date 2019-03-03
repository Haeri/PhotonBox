#ifndef DEBUG_GUI_H
#define DEBUG_GUI_H

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "imgui/ImGuizmo.h"

class DebugGUI
{
public:
	void init();
	void newFrame();
	void render();
	void destroy();
};

#endif // DEBUG_GUI_H

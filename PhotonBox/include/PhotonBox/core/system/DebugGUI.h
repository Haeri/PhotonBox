#ifndef DEBUG_GUI_H
#define DEBUG_GUI_H

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "PhotonBox/core/ISystem.h"

class DebugGUI : public ISystem
{
public:
	void init(Config::Profile profile) override;
	void start() override;
	void reset() override;
	void destroy() override;

	void newFrame();
	void render();
};

#endif // DEBUG_GUI_H

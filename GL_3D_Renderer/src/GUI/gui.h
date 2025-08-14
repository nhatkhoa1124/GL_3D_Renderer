#pragma once

#include <imgui.h>

#include <vector>
#include <memory>
#include "light.h"
#include "GLFW/glfw3.h"

namespace GUI {
	void Init(GLFWwindow* window);
	void Begin();
	void End();
	void Shutdown();
	void Render(std::vector<std::unique_ptr<Light>>& lights);
};

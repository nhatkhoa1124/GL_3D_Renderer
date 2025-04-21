#pragma once

#include <imgui.h>

#include <vector>
#include <memory>
#include "light.h"

namespace GUI {
	void Init();
	void Begin();
	void End();
	void Shutdown();
	void Render(std::vector<std::unique_ptr<Light>>& lights);
};

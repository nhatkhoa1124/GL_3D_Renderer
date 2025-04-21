#pragma once

#include<vector>
#include <memory>
#include "light.h"

namespace LightingGUI {
	void Render(std::vector<std::unique_ptr<Light>>& lights);
}
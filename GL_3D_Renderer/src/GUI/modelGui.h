#pragma once

#include<vector>
#include <memory>
#include "model.h"

namespace ModelGUI {
	void Render(std::vector<std::unique_ptr<Model::Model>>& models);
}
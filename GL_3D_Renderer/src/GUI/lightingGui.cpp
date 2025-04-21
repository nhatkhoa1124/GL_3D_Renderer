#include "lightingGui.h"
#include <imgui.h>

#include "core.h"

void LightingGUI::Render(std::vector<std::unique_ptr<Light>>& lights)
{
	ImGui::Begin("Lighting");

	static float dirLightColor[3] = { 1.0f, 1.0f, 1.0f };
	static float pointLightColor[3] = { 1.0f, 1.0f, 1.0f };

	ImGui::ColorEdit3("Directional light color", dirLightColor);
	ImGui::ColorEdit3("Point light color", pointLightColor);

	for (size_t i = 0; i < lights.size(); i++)
	{
		if (lights[i]->getLightType() == LightType::DIRECTIONAL)
		{
			lights[i]->setColor(glm::vec3(dirLightColor[0], dirLightColor[1], dirLightColor[2]));
		}
		else if (lights[i]->getLightType() == LightType::POINT)
		{
			lights[i]->setColor(glm::vec3(pointLightColor[0], pointLightColor[1], pointLightColor[2]));
		}
	}

	ImGui::End();
}

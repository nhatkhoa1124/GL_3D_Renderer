#include "core.h"

#include "scene.h"
#include "vertex.h"
#include "vertexBuffer.h"
#include "vertexArray.h"
#include "shaderProgram.h"
#include "camera.h"
#include "texture.h"
#include "model.h"
#include "stb_image.h"
#include "directionalLight.h"
#include "pointLight.h"
#include "gui.h"

void mouse_callback(GLFWwindow* window, double xoffset, double yoffset);

Camera* gCamera = nullptr;
int main() {
	// Initalize
	SceneManager::Scene scene = {};
	scene.init();
	glfwSetCursorPosCallback(scene.mWindow, mouse_callback);

	// Setup Shaders
	std::string vertexShaderPath = "src/shaders/vertexShader.vert";
	std::string fragmentShaderPath = "src/shaders/phong.frag";
	ShaderProgram shaderProgram = { vertexShaderPath , fragmentShaderPath };
	shaderProgram.useProgram();

	// Setup lighting
	std::vector<std::unique_ptr<Light>> lightList;
	lightList.push_back(std::make_unique<DirectionalLight>(glm::vec3(1.0f)));
	lightList.push_back(std::make_unique<PointLight>(glm::vec3(1.0f, 6.0f, 0.0f), 1.0f, 0.045f, 0.0075f));
	lightList.push_back(std::make_unique<PointLight>(glm::vec3(0.0f, 0.0f, 2.0f), 1.0f, 0.045f, 0.0075f));

	// Setup Camera
	Camera camera = { glm::vec3(0.0f, 0.0f, 6.0f) };
	gCamera = &camera;
	float deltaTime = 0.02f;

	// Load model
	Model::Model dragon = { "assets/fbx/Dragon 2.5_fbx.fbx", false };
	Model::Model backpack = { "assets/backpack/backpack.obj", true };

	GUI::Init();

	while (!glfwWindowShouldClose(scene.mWindow))
	{
		if (glfwGetKey(scene.mWindow, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(scene.mWindow, true);
		}

		glClearColor(0.13f, 0.15f, 0.23f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GUI::Begin();
		shaderProgram.useProgram();

		// Process lighting
		int pointIndex = 0;
		for (auto& light : lightList) {
			LightType type = light->getLightType();

			if (type == LightType::DIRECTIONAL) {
				light->setShaderLight(shaderProgram, "dirLight");
			}
			else if (type == LightType::POINT) {
				// Count how many point lights rendered so far
				std::string name = "pointLight[" + std::to_string(pointIndex++) + "]";
				light->setShaderLight(shaderProgram, name.c_str());
			}
		}
		shaderProgram.setUniformVec3(camera.getPos(), "viewPos");
		shaderProgram.setUniformVec3(glm::vec3(0.05f), "phongIntensity.ambient");
		shaderProgram.setUniformVec3(glm::vec3(0.8f), "phongIntensity.diffuse");
		shaderProgram.setUniformVec3(glm::vec3(1.0f), "phongIntensity.specular");

		// Process camera
		camera.processKeyboard(scene.mWindow, deltaTime);
		// Process matrices and transformation
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), static_cast<float>(scene.WINDOW_WIDTH) / static_cast<float>(scene.WINDOW_HEIGHT), 0.1f, 100.0f);
		glm::mat4 viewMatrix = camera.getViewMatrix();
		shaderProgram.setMVP(modelMatrix, viewMatrix, projectionMatrix);
		backpack.drawModel(shaderProgram);

		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 3.0f, -4.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
		shaderProgram.setMVP(modelMatrix, viewMatrix, projectionMatrix);
		dragon.drawModel(shaderProgram);

		GUI::Render(lightList);
		GUI::End();

		scene.update();
	}

	GUI::Shutdown();
	scene.exit();
	return 0;
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	static bool isMousePressed = false;
	static float lastX;
	static float lastY;
	bool isImGuiActive = ImGui::GetIO().WantCaptureMouse;

	if (!isImGuiActive) {
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			if (!isMousePressed) {
				isMousePressed = true;

				lastX = xpos;
				lastY = ypos;
			}


			float xOffset = xpos - lastX;
			float yOffset = lastY - ypos;

			if (gCamera) {
				gCamera->processCameraMovement(xOffset, yOffset);
			}

			lastX = xpos;
			lastY = ypos;
		}
		else {
			isMousePressed = false;
		}
	}
}
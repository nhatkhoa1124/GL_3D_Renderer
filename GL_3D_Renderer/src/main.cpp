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
	Scene::init();
	glfwSetCursorPosCallback(Scene::mWindow, mouse_callback);
	GUI::Init();

	// Setup Shaders
	const std::string vertexShaderPath = "src/shaders/vertexShader.vert";
	const std::string fragmentShaderPath = "src/shaders/phong.frag";
	ShaderProgram shaderProgram = { vertexShaderPath , fragmentShaderPath };
	shaderProgram.useProgram();

	// Setup lighting
	std::vector<std::unique_ptr<Light>> lightList;
	lightList.push_back(std::make_unique<DirectionalLight>(glm::vec3(0.0f, -1.0f, -1.0f)));
	lightList.push_back(std::make_unique<PointLight>(glm::vec3(1.0f, 6.0f, 0.0f), 1.0f, 0.045f, 0.0075f));
	lightList.push_back(std::make_unique<PointLight>(glm::vec3(0.0f, 0.0f, 2.0f), 1.0f, 0.045f, 0.0075f));

	// Setup Camera
	Camera camera = { glm::vec3(0.0f, 0.0f, 6.0f) };
	gCamera = &camera;
	float deltaTime = 0.02f;

	// Load model
	Model::Model table = { "assets/table/table.obj", true };
	Model::Model dragon = { "assets/fbx/Dragon 2.5_fbx.fbx", false };
	Model::Model backpack = { "assets/backpack/backpack.obj", true };
	Model::Model cup = { "assets/GlassCup/Cup_Made_By_Tyro_Smith.ply", false };


	while (!glfwWindowShouldClose(Scene::mWindow))
	{
		if (glfwGetKey(Scene::mWindow, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(Scene::mWindow, true);
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
		camera.processKeyboard(Scene::mWindow, deltaTime);
		// Process matrices and transformation
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT), 0.1f, 100.0f);
		glm::mat4 viewMatrix = camera.getViewMatrix();

		// Draw backpack
		glm::mat4 modelMatrixBackpack = glm::mat4(1.0f);
		modelMatrixBackpack = glm::translate(modelMatrixBackpack, glm::vec3(0.0f, 0.0f, 0.0f));
		modelMatrixBackpack = glm::scale(modelMatrixBackpack, glm::vec3(1.0f, 1.0f, 1.0f));
		shaderProgram.setMVP(modelMatrixBackpack, viewMatrix, projectionMatrix);
		backpack.drawModel(shaderProgram);
		// Draw table
		glm::mat4 modelMatrixTable = glm::mat4(1.0f);
		modelMatrixTable = glm::translate(modelMatrixTable, glm::vec3(0.0f, -5.0f, -4.0f));
		modelMatrixTable = glm::scale(modelMatrixTable, glm::vec3(0.05f, 0.05f, 0.05f));
		shaderProgram.setMVP(modelMatrixTable, viewMatrix, projectionMatrix);
		table.drawModel(shaderProgram);
		// Draw dragon
		glm::mat4 modelMatrixDragon = glm::mat4(1.0f);
		modelMatrixDragon = glm::translate(modelMatrixDragon, glm::vec3(0.0f, 3.0f, -4.0f));
		modelMatrixDragon = glm::rotate(modelMatrixDragon, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrixDragon = glm::scale(modelMatrixDragon, glm::vec3(0.1f, 0.1f, 0.1f));
		shaderProgram.setMVP(modelMatrixDragon, viewMatrix, projectionMatrix);
		dragon.drawModel(shaderProgram);
		// Draw cup
		glm::mat4 modelMatrixCup = glm::mat4(1.0f);
		modelMatrixCup = glm::translate(modelMatrixCup, glm::vec3(0.0f, -2.0f, 2.0f));
		modelMatrixCup = glm::rotate(modelMatrixCup, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrixCup = glm::scale(modelMatrixCup, glm::vec3(0.3f, 0.3f, 0.3f));
		shaderProgram.setMVP(modelMatrixCup, viewMatrix, projectionMatrix);
		cup.drawModel(shaderProgram);


		GUI::Render(lightList);
		GUI::End();
		Scene::update();
	}

	GUI::Shutdown();
	Scene::exit();
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
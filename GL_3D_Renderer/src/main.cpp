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

void mouse_callback(GLFWwindow* window, double xoffset, double yoffset);

Camera* gCamera = nullptr;
int main() {
	// Initalize
	SceneManager::Scene scene = {};
	scene.init();

	// Setup Shaders
	std::string vertexShaderPath = "src/shaders/vertexShader.vert";
	std::string fragmentShaderPath = "src/shaders/phong.frag";
	ShaderProgram shaderProgram = { vertexShaderPath , fragmentShaderPath };
	shaderProgram.useProgram();

	// Setup lighting
	DirectionalLight dirLight = { glm::vec3(1.0f) };
	dirLight.setColor(glm::vec3(0.2f, 0.5f, 0.7f));
	std::vector<PointLight> pointLights = {
		{glm::vec3(1.0f, 6.0f, 0.0f), 1.0f, 0.045f, 0.0075f} ,
		{glm::vec3(0.0f, 0.0f, 2.0f), 1.0f, 0.045f, 0.0075f}
	};

	// Setup Camera
	Camera camera = { glm::vec3(0.0f, 0.0f, 6.0f) };
	gCamera = &camera;
	float deltaTime = 0.02f;
	glfwSetCursorPosCallback(scene.mWindow, mouse_callback);

	// Load model
	Model::Model dragon = { "assets/fbx/Dragon 2.5_fbx.fbx", false };
	Model::Model backpack = { "assets/backpack/backpack.obj", true };


	while (!glfwWindowShouldClose(scene.mWindow))
	{
		if (glfwGetKey(scene.mWindow, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(scene.mWindow, true);
		}

		glClearColor(0.13f, 0.15f, 0.23f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		shaderProgram.useProgram();

		// Process lighting
		dirLight.setShaderLight(shaderProgram, "dirLight");
		for (size_t i = 0; i < pointLights.size(); i++)
		{
			std::string idx = "pointLight[" + std::to_string(i) + "]";
			glm::mat4 pointLightModel = glm::mat4(1.0f);
			pointLightModel = glm::rotate(pointLightModel, glm::radians(3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::vec4 newPos = glm::vec4(pointLights[i].getPosition(), 1.0f) * pointLightModel;
			pointLights[i].setPosition(glm::vec3(newPos));
			pointLights[i].setShaderLight(shaderProgram, idx.c_str());
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

		scene.update();
	}

	scene.exit();
	return 0;
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	static bool firstMouse = true;
	static float lastX;
	static float lastY;

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;

	if (gCamera) {
		gCamera->processCameraMovement(xOffset, yOffset);
	}

	lastX = xpos;
	lastY = ypos;
}


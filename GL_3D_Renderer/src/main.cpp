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

void mouse_callback(GLFWwindow* window, double xoffset, double yoffset);

Camera* gCamera = nullptr;
int main() {
	// Initalize
	SceneManager::Scene scene = {};
	scene.init();

	// Setup Shaders
	std::string vertexShaderPath = "src/shaders/vertexShader.vert";
	std::string fragmentShaderPath = "src/shaders/fragmentShader.frag";
	ShaderProgram shaderProgram = { vertexShaderPath , fragmentShaderPath };
	shaderProgram.useProgram();


	// Setup Camera
	Camera camera = { glm::vec3(0.0f, 0.0f, 6.0f) };
	gCamera = &camera;
	float deltaTime = 0.02f;
	glfwSetCursorPosCallback(scene.mWindow, mouse_callback);

	// Load model
	Vertex::Model model = { "assets/backpack/backpack.obj" };


	while (!glfwWindowShouldClose(scene.mWindow))
	{
		if (glfwGetKey(scene.mWindow, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(scene.mWindow, true);
		}

		glClearColor(0.13f, 0.15f, 0.23f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		shaderProgram.useProgram();


		// Process camera
		camera.processKeyboard(scene.mWindow, deltaTime);


		// Process matrices and transformation
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(50.0f), static_cast<float>(scene.WINDOW_WIDTH) / static_cast<float>(scene.WINDOW_HEIGHT), 0.1f, 100.0f);
		glm::mat4 viewMatrix = camera.getViewMatrix();

		shaderProgram.setMVP(modelMatrix, viewMatrix, projectionMatrix);


		// Process lighting (if applicable)

		model.drawModel(shaderProgram);


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


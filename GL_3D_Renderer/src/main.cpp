#include "core.h"
#include "scene.h"
#include "vertex.h"
#include "vertexBuffer.h"
#include "vertexArray.h"
#include "shaderProgram.h"
#include "camera.h"

void mouse_callback(GLFWwindow* window, double xoffset, double yoffset);

Camera* gCamera = nullptr;

int main() {
	SceneManager::Scene scene = {};
	scene.init();

	Vertex::VertexBuffer VertexBuffer = {};
	Vertex::VertexArray VertexArray = {};
	VertexBuffer.bindVertexBuffer(Vertex::rawCube);
	VertexArray.bindVertexArray(VertexBuffer.getVertexBufferObject());

	std::string vertexShaderPath = "src/shaders/vertexShader.vert";
	std::string fragmentShaderPath = "src/shaders/fragmentShader.frag";
	ShaderProgram shaderProgram = { vertexShaderPath , fragmentShaderPath };
	shaderProgram.useProgram();

	Camera camera = { glm::vec3(1.0f, 0.3f, 4.0f) };
	gCamera = &camera;
	float deltaTime = 0.02f;

	glfwSetInputMode(scene.mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(scene.mWindow, mouse_callback);

	// Temporary matrices
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	GLuint mvpLoc = glGetUniformLocation(shaderProgram.getProgramId(), "mvp");

	while (!glfwWindowShouldClose(scene.mWindow))
	{
		if (glfwGetKey(scene.mWindow, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(scene.mWindow, true);
		}
		glClearColor(0.13f, 0.15f, 0.23f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Process camera
		camera.processKeyboard(scene.mWindow, deltaTime);

		glm::mat4 viewMatrix = camera.getViewMatrix();
		glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

		shaderProgram.useProgram();
		VertexArray.bindVertexArray(VertexBuffer.getVertexBufferObject());

		glDrawArrays(GL_TRIANGLES, 0, 36);
		scene.update();
	}

	VertexBuffer.deleteVertexBuffer();
	VertexArray.deleteVertexArray();

	scene.exit();
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	static bool firstMouse = true;
	static float lastX = 400.0f;
	static float lastY = 300.0f;

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xOffset = lastX - xpos;
	float yOffset = ypos - lastY;

	if (gCamera) {
		gCamera->processCameraMovement(xOffset, yOffset);
	}

	lastX = xpos;
	lastY = ypos;
}
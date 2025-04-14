#include "core.h"

#include "scene.h"
#include "vertex.h"
#include "vertexBuffer.h"
#include "vertexArray.h"
#include "shaderProgram.h"
#include "camera.h"
#include "texture.h"

void mouse_callback(GLFWwindow* window, double xoffset, double yoffset);

Camera* gCamera = nullptr;

int main() {
	// Initalize
	SceneManager::Scene scene = {};
	scene.init();

	// VBO & VAO
	Vertex::VertexBuffer VertexBuffer = {};
	Vertex::VertexArray VertexArray = {};
	VertexBuffer.bindVertexBuffer(Vertex::rawCube);
	VertexArray.bindVertexArray(VertexBuffer.getVertexBufferObject());

	// Setup Shaders
	std::string vertexShaderPath = "src/shaders/vertexShader.vert";
	std::string fragmentShaderPath = "src/shaders/fragmentShader.frag";
	ShaderProgram shaderProgram = { vertexShaderPath , fragmentShaderPath };
	shaderProgram.useProgram();

	// Setup Camera
	Camera camera = { glm::vec3(0.0f, 0.0f, 3.0f) };
	gCamera = &camera;
	float deltaTime = 0.02f;
	glfwSetCursorPosCallback(scene.mWindow, mouse_callback);

	// Load Textures
	Texture texture = {};
	texture.loadTexture("assets/cobblestone.png");

	// Temporary matrices
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), static_cast<float>(scene.WINDOW_WIDTH) / static_cast<float>(scene.WINDOW_HEIGHT), 0.1f, 100.0f);
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
		texture.bindTexture();
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
	static float lastX;
	static float lastY;

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
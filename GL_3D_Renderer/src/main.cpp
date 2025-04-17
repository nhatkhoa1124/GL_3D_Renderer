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
	std::string fragmentShaderPath = "src/shaders/phong.frag";
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

	// Matrix values
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), static_cast<float>(scene.WINDOW_WIDTH) / static_cast<float>(scene.WINDOW_HEIGHT), 0.1f, 100.0f);

	// Setup lighting & materials
	glm::vec3 tempLightPos = glm::vec3(0.0f, 1.0f, 5.0f);
	glm::vec3 tempLightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	while (!glfwWindowShouldClose(scene.mWindow))
	{
		if (glfwGetKey(scene.mWindow, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(scene.mWindow, true);
		}
		glClearColor(0.13f, 0.15f, 0.23f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Process camera
		camera.processKeyboard(scene.mWindow, deltaTime);

		// Process matrices and transformation
		//float rotateAngle = glm::sin(glfwGetTime()) / 2.0f + 0.5f;
		//tempLightPos = glm::vec4(tempLightPos, 1.0) * glm::rotate(modelMatrix, glm::radians(2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 viewMatrix = camera.getViewMatrix();
		shaderProgram.setMVP(modelMatrix, viewMatrix, projectionMatrix);

		// Process lighting
		shaderProgram.setUniformVec3(camera.getPos(), "viewPos");
		shaderProgram.setUniformVec3(tempLightPos, "light.lightPos");
		shaderProgram.setUniformVec3(tempLightColor, "light.lightColor");
		shaderProgram.setUniformFloat(0.2f, "material.ambientStrength");
		shaderProgram.setUniformFloat(0.89f, "material.diffuseStrength");
		shaderProgram.setUniformFloat(1.0f, "material.specularStrength");
		shaderProgram.setUniformFloat(32.0f, "material.shininess");

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

	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;

	if (gCamera) {
		gCamera->processCameraMovement(xOffset, yOffset);
	}

	lastX = xpos;
	lastY = ypos;
}
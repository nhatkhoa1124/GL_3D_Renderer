#include <iostream>

#include "scene.h"
#include "gui.h"
#include "core.h"
#include "glad/glad.h"
#include "shaderProgram.h"
#include "camera.h"
#include "vertex.h"
#include "vertexBuffer.h"
#include "vertexArray.h"
#include "texture.h"
#include "model.h"
#include "stb_image.h"
#include "directionalLight.h"
#include "frameBuffer.h"
#include "cubemap.h"

namespace SceneManager {
	Scene::Scene() :
		mDeltaTime{ 0.02f },
		mWindow{ nullptr },
		mCamera{ Camera{glm::vec3(0.0f, 0.0f, 6.0f)} },
		mSkybox{},
		mShadow{},
		mPointShadow{},
		mCameraProjection
		{
			glm::perspective
			(
				glm::radians(45.0f),
				static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT),
				0.1f,
				100.0f
			)
		},
		mLightList{},
		mModelList{}
	{
		mLightList.reserve(3);
		mModelList.reserve(8);
	}

	Scene::~Scene()
	{
		exit();
	}

	static void error_callback(int error, const char* description)
	{
		fprintf(stderr, "Error: %s\n", description);
	}

	void Scene::init() {
		glfwSetErrorCallback(error_callback);

		if (!glfwInit()) {
			std::cerr << "FAILED TO INITALIZE GLFW\n";
			glfwTerminate();
			return;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
		mWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "3d_Renderer", nullptr, nullptr);
		if (mWindow == nullptr) {
			std::cerr << "FAILED TO CREATE WINDOW\n";
			glfwTerminate();
		}
		glfwMakeContextCurrent(mWindow);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cerr << "FAILED TO INITIALIZE GLAD\n";
		}

		glfwSetWindowUserPointer(mWindow, this);
		glfwSetCursorPosCallback(mWindow, &Scene::staticMouseCallback);
		GUI::Init(mWindow);
		setupScene();

	}

	void Scene::setupScene()
	{
		CreateLighting();
		LoadModel();
		// Setup skybox
		std::vector<std::string> faces =
		{
			"assets/skybox/right.jpg",
			"assets/skybox/left.jpg",
			"assets/skybox/top.jpg",
			"assets/skybox/bottom.jpg",
			"assets/skybox/front.jpg",
			"assets/skybox/back.jpg",
		};
		mSkybox.loadTexture(faces, false);
		// Setup shadow
		mShadow.init();
		mPointShadow.init();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_MULTISAMPLE);
		//glEnable(GL_FRAMEBUFFER_SRGB); // Gamma correction
	}

	void Scene::renderScene(ShaderProgram& shader)
	{
		for (const auto& m : mModelList)
		{
			shader.setMVP(m.getModelMatrix(), mCamera.getViewMatrix(), mCameraProjection);
			m.drawModel(shader);
		}
	}

	void Scene::renderSkybox(ShaderProgram& skyboxShader)
	{
		glDepthFunc(GL_LEQUAL);
		glm::mat4 skyboxView = glm::mat4(glm::mat3(mCamera.getViewMatrix()));
		skyboxShader.useProgram();
		skyboxShader.setUniformMat4(skyboxView, "view");
		skyboxShader.setUniformMat4(mCameraProjection, "projection");
		mSkybox.drawCube();
		glDepthFunc(GL_LESS);
	}

	void Scene::renderDepthMap(ShaderProgram& depthMapShader)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mShadow.getFramebuffer());
		glClear(GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_FRONT);
		renderScene(depthMapShader);
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Scene::renderCubeDepthMap(ShaderProgram& cubeDepthMapShader, PointLight* pointLight)
	{
		if (!pointLight) return;
		mPointShadow.setLightPosition(pointLight->getPosition());

		auto shadowTransforms = mPointShadow.getShadowTransforms();
		int shadowSize = mPointShadow.getShadowSize();
		glViewport(0, 0, shadowSize, shadowSize);
		glBindFramebuffer(GL_FRAMEBUFFER, mPointShadow.getFrameBuffer());
		glClear(GL_DEPTH_BUFFER_BIT);
		cubeDepthMapShader.useProgram();
		for (int i = 0; i < 6; i++)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				mPointShadow.getDepthMap(), 0);
			cubeDepthMapShader.setUniformMat4(shadowTransforms[i], "shadowMatrix");

		}
		cubeDepthMapShader.setUniformVec3(pointLight->getPosition(), "lightPos");
		cubeDepthMapShader.setUniformFloat(mPointShadow.getFarPlane(), "farPlane");
		renderScene(cubeDepthMapShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Scene::LoadModel()
	{
		Model::Model priestess = { "assets/altan_avian_priestess/scene.gltf", false };
		Model::Model chinese_statue = { "assets/funny_chinese_statue/scene.gltf", false };
		Model::Model table = {"assets/a_table/scene.gltf", false};
		
		// Setup model matrix for each models
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f, 1.53f, -4.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(2.3f, 2.3f, 2.3f));
		priestess.setModelMatrix(modelMatrix);
		
		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(3.0f, -0.56f, -2.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.015f, 0.015f, 0.015f));
		chinese_statue.setModelMatrix(modelMatrix);

		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -10.0f, -3.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		table.setModelMatrix(modelMatrix);

		mModelList.push_back(priestess);
		mModelList.push_back(chinese_statue);
		mModelList.push_back(table);
	}

	void Scene::CreateLighting()
	{
		mLightList.push_back(std::make_unique<DirectionalLight>(glm::vec3(0.0f, -1.0f, -1.0f)));
		mLightList.push_back(std::make_unique<PointLight>(glm::vec3(0.0f, 5.3f, 0.4f), 1.0f, 0.007f, 0.0002f));
	}

	void Scene::update() {
		// Setup
		ShaderProgram sceneShader = { "src/shaders/vertexShader.vert" , "src/shaders/blinnphong.frag" };
		ShaderProgram skyboxShader = { "src/shaders/skybox.vert" , "src/shaders/skybox.frag" };
		ShaderProgram depthMapShader = { "src/shaders/depthmap.vert", "src/shaders/blankshader.frag" };
		ShaderProgram pointShadowShader = { "src/shaders/point_shadow.vert", "src/shaders/blankshader.frag" };

		// Define light view's near & far plane
		float near = 1.0f, far = 25.0f;

		// Render
		while (!glfwWindowShouldClose(Scene::mWindow))
		{
			int width, height;
			glfwGetFramebufferSize(mWindow, &width, &height);
			if (height > 0) { // Prevent division by zero if window is minimized
				mCameraProjection = glm::perspective(
					glm::radians(45.0f),
					static_cast<float>(width) / static_cast<float>(height),
					0.1f,
					100.0f
				);
			}

			if (glfwGetKey(Scene::mWindow, GLFW_KEY_ESCAPE)) {
				glfwSetWindowShouldClose(Scene::mWindow, true);
			}
			GUI::Begin();

			// --Render depth map & shadow
			glm::mat4 lightProjection = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, near, far);
			glm::mat4 lightView = glm::lookAt
			(
				glm::vec3(0.0f, 5.0f, 5.0f), // Position of light = -lightDir * distance
				glm::vec3(0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f)
			);
			glm::mat4 lightMatrix = lightProjection * lightView;
			depthMapShader.useProgram();
			depthMapShader.setUniformMat4(lightMatrix, "lightMatrix");
			glViewport(0, 0, mShadow.getWidth(), mShadow.getHeight());
			renderDepthMap(depthMapShader); // Create depth map
			// Create Cube depth map
			int pointLightIndex = 0;
			for (auto& light : mLightList) {
				if (light->getLightType() == LightType::POINT) {
					PointLight* pointLight = dynamic_cast<PointLight*>(light.get());
					renderCubeDepthMap(pointShadowShader, pointLight);
					pointLightIndex++;
				}
			}

			// --Render main scene
			glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
			sceneShader.useProgram();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// Process camera
			mCamera.processKeyboard(Scene::mWindow, mDeltaTime);
			// Define view matrix
			glm::mat4 viewMatrix = mCamera.getViewMatrix();
			// Process lighting
			int pointIndex = 0;
			for (auto& light : mLightList) {
				LightType type = light->getLightType();

				if (type == LightType::DIRECTIONAL) {
					light->setShaderLight(sceneShader, "dirLight");
				}
				else if (type == LightType::POINT) {
					// Count how many point lights rendered so far
					std::string name = "pointLight[" + std::to_string(pointIndex++) + "]";
					light->setShaderLight(sceneShader, name.c_str());
				}
			}
			sceneShader.setUniformVec3(mCamera.getPos(), "viewPos");
			sceneShader.setUniformVec3(glm::vec3(0.03f), "phongIntensity.ambient");
			sceneShader.setUniformVec3(glm::vec3(0.5f), "phongIntensity.diffuse");
			sceneShader.setUniformVec3(glm::vec3(1.0f), "phongIntensity.specular");
			sceneShader.setUniformMat4(lightMatrix, "lightMatrix");

			sceneShader.setUniformInt(8, "shadowMap");
			glActiveTexture(GL_TEXTURE8);
			glBindTexture(GL_TEXTURE_2D, mShadow.getDepthmap());
			sceneShader.setUniformInt(9, "pointShadowMap");
			glActiveTexture(GL_TEXTURE9);
			glBindTexture(GL_TEXTURE_CUBE_MAP, mPointShadow.getDepthMap());
			sceneShader.setUniformFloat(mPointShadow.getFarPlane(), "farPlane");

			renderScene(sceneShader);
			renderSkybox(skyboxShader);

			GUI::Render(mLightList);
			GUI::End();
			glfwSwapBuffers(mWindow);
			glfwPollEvents();
		}

	}

	void Scene::exit() {
		GUI::Shutdown();
		if (mWindow) {
			glfwDestroyWindow(mWindow);
			mWindow = nullptr;
		}
		glfwTerminate();
	}


	void Scene::staticMouseCallback(GLFWwindow* window, double xpos, double ypos)
	{
		Scene* scene = static_cast<Scene*>(glfwGetWindowUserPointer(window));
		scene->instanceMouseCallback(xpos, ypos);
	}

	void Scene::instanceMouseCallback(double xpos, double ypos)
	{
		static bool isMousePressed = false;
		static float lastX;
		static float lastY;
		bool isImGuiActive = ImGui::GetIO().WantCaptureMouse;
		if (!isImGuiActive) {
			if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
				if (!isMousePressed) {
					isMousePressed = true;

					lastX = xpos;
					lastY = ypos;
				}
				float xOffset = xpos - lastX;
				float yOffset = lastY - ypos;
				mCamera.processCameraMovement(xOffset, yOffset);
				lastX = xpos;
				lastY = ypos;
			}
			else {
				isMousePressed = false;
			}
		}
	}
}
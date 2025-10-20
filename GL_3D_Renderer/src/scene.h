#pragma once

#define GLFW_INCLUDE_NONE
#include "camera.h"
#include "cubemap.h"
#include "shadow.h"
#include "light.h"
#include "pointLight.h"
#include "model.h"
#include "shadowCubeMap.h"

namespace SceneManager {
	class Scene {
	public:
		Scene();
		~Scene();
		void init();
		void update();
		void exit();
	private:
		void setupScene();
		void renderScene(ShaderProgram& shader);
		void renderSkybox(ShaderProgram& skyboxShader);
		void renderDepthMap(ShaderProgram& depthMapshader);
		void renderCubeDepthMap(ShaderProgram& cubeDepthMapshader, PointLight* pointLight);

		void LoadModel();
		void CreateLighting();


	private:
		float mDeltaTime; // Delta time still not calculated properly
		GLFWwindow* mWindow;
		Camera mCamera;
		CubeMap mSkybox;
		Shadow mShadow;
		ShadowCubeMap mPointShadow;

		glm::mat4 mCameraProjection;
		std::vector<std::unique_ptr<Light>> mLightList;
		std::vector<Model::Model> mModelList;

		static void staticMouseCallback(GLFWwindow* window, double xpos, double ypos);
		void instanceMouseCallback(double xpos, double ypos);
	};
}
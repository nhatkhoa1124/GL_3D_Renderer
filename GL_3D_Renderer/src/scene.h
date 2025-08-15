#pragma once

#define GLFW_INCLUDE_NONE
#include <camera.h>
#include <cubemap.h>
#include <shadow.h>
#include <light.h>
#include <model.h>

namespace SceneManager {
	class Scene {
	public:
		Scene();
		~Scene();
		void init();
		void update();
		void exit();

	private:
		float mDeltaTime;
		GLFWwindow* mWindow;
		Camera mCamera;
		CubeMap mSkybox;
		Shadow mShadow;
		glm::mat4 mCameraProjection;
		std::vector<std::unique_ptr<Light>> mLightList;
		std::vector<Model::Model> mModelList;
		void setupScene();
		void renderScene(ShaderProgram& shader);
		void renderSkybox(ShaderProgram& shader);
		void renderDepthMap(ShaderProgram& shader);

		static void staticMouseCallback(GLFWwindow* window, double xpos, double ypos);
		void instanceMouseCallback(double xpos, double ypos);
	};
}
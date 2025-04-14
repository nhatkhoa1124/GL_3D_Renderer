#include "scene.h"

namespace SceneManager {
	Scene::Scene() { mWindow = nullptr; }

	Scene::~Scene() { exit(); }

	static void error_callback(int error, const char* description)
	{
		fprintf(stderr, "Error: %s\n", description);
	}

	void Scene::init() {
		glfwSetErrorCallback(error_callback);

		if (!glfwInit()) {
			std::cerr << "FAILED TO INITALIZE GLFW\n";
			glfwTerminate();
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		mWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "3d_Renderer", nullptr, nullptr);
		if (mWindow == nullptr) {
			std::cerr << "FAILED TO CREATE WINDOW\n";
			glfwTerminate();
		}
		glfwMakeContextCurrent(mWindow);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cerr << "FAILED TO INITIALIZE GLAD\n";
		}

		glEnable(GL_DEPTH_TEST);
	}

	void Scene::update() {
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}

	void Scene::exit() {
		if (mWindow) {
			glfwDestroyWindow(mWindow);
			mWindow = nullptr;
		}
		glfwTerminate();
	}
}
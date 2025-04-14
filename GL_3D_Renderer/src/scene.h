#pragma once

#include "core.h"

namespace SceneManager {
	class Scene {
	public:
		GLFWwindow* mWindow;
		const GLint WINDOW_WIDTH = 800;
		const GLint WINDOW_HEIGHT = 600;

		Scene();
		~Scene();
		void init();
		void update();
		void exit();
	};
}
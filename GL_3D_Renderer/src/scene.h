#pragma once

namespace SceneManager {
	class Scene {
	public:
		GLFWwindow* mWindow;
		static constexpr GLint WINDOW_WIDTH = 800;
		static constexpr GLint WINDOW_HEIGHT = 600;

		Scene();
		~Scene();
		void init();
		void update();
		void exit();
	};
}
#pragma once

namespace SceneManager {
	class Scene {
	public:
		GLFWwindow* mWindow;
		static constexpr GLint WINDOW_WIDTH = 1280;
		static constexpr GLint WINDOW_HEIGHT = 720;

		Scene();
		~Scene();
		void init();
		void update();
		void exit();
	};
}
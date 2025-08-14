#include "scene.h"
int main() {
	// Project initialization
	SceneManager::Scene scene = {};
	scene.init();
	scene.update();
	scene.exit();
}

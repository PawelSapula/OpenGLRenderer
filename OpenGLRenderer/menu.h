#pragma once
#include <GLFW/glfw3.h>

namespace Menu {

	void init(GLFWwindow* window);
	void frameSetup();
	void render();
	void shutdown();

}
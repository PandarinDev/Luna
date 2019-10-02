#include "InputManager.h"

#include <iostream>

namespace luna {

	InputManager::InputManager(const Window& window) {
		glfwSetKeyCallback(window.getHandle(), [&](GLFWwindow* window, int key, int scancode, int action, int mods) {
			keyPressed(key);
		});
	}

}
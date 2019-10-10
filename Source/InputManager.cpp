#include "InputManager.h"

#include <iostream>

namespace luna {

	GLFWwindow* InputManager::WINDOW = nullptr;

	void InputManager::initialize(const Window& window) {
		InputManager::WINDOW = window.getHandle();
	}
	
	InputManager& InputManager::getInstance() {
		static InputManager instance;
		return instance;
	}

	void InputManager::keyHandler(GLFWwindow* window, int key, int scancode, int action, int mods) {
		auto& instance = getInstance();
		// TODO Handle input events
	}

	InputManager::InputManager() {
		glfwSetKeyCallback(WINDOW, InputManager::keyHandler);
	}

}
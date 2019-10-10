#pragma once

#include "Window.h"

namespace luna {

	class InputManager {

	public:

		static void initialize(const Window& window);
		static InputManager& getInstance();

	private:

		static GLFWwindow* WINDOW;

		static void keyHandler(GLFWwindow* window, int key, int scancode, int action, int mods);

		InputManager();

	};

}
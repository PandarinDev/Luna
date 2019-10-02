#include "Window.h"

#include <stdexcept>

namespace luna {

	Window::Window(const std::string& title, int width, int height, bool vSync, bool fullScreen) :
		handle(nullptr), title(title), width(width), height(height), vSync(vSync), fullScreen(fullScreen) {
		if (!glfwInit()) {
			throw std::runtime_error("Failed to initialize GLFW.");
		}

		handle = glfwCreateWindow(width, height, title.c_str(),
			fullScreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
		if (!handle) {
			throw std::runtime_error("Failed to create GLFW window.");
		}
		glfwMakeContextCurrent(handle);
		glfwSwapInterval(vSync ? 1 : 0);
	}

	void Window::pollEvents() const {
		glfwPollEvents();
	}

	void Window::swapBuffers() const {
		glfwSwapBuffers(handle);
	}

	bool Window::shouldClose() const {
		return glfwWindowShouldClose(handle);
	}

	GLFWwindow* Window::getHandle() const {
		return handle;
	}

}
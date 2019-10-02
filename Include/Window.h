#pragma once

#include <GLFW/glfw3.h>

#include <string>

namespace luna {

	class Window {

	public:

		Window(const std::string& title, int width, int height, bool vSync, bool fullScreen);

		void pollEvents() const;
		void swapBuffers() const;
		bool shouldClose() const;

		GLFWwindow* getHandle() const;

	private:

		GLFWwindow* handle;
		std::string title;
		int width;
		int height;
		bool vSync;
		bool fullScreen;

	};

}
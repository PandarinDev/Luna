#pragma once

#include <GLFW/glfw3.h>

namespace luna {

	class PlatformUtils {

	public:

		static float getTime() {
			return glfwGetTime();
		}

	private:

		PlatformUtils() = default;

	};

}
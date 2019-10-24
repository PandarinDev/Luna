#include "Timer.h"

#include <GLFW/glfw3.h>

#include <iostream>

namespace luna {

	Timer::Timer() :
		fps(0.0f), delta(0.0f), frames(0) {
		float time = getTime();
		lastFrame = time;
		lastFps = time;
	}

	float Timer::getTime() const {
		return glfwGetTime();
	}

	float Timer::getDelta() const {
		return delta;
	}

	int Timer::getFps() const {
		return fps;
	}

	void Timer::tick() {
		float time = getTime();
		if (time - lastFps > 1.0f) {
			fps = frames;
			frames = 0;
			lastFps = time;
			std::cout << "FPS is: " << fps << std::endl;
		}
		delta = time - lastFrame;
		lastFrame = time;
		++frames;
	}

}

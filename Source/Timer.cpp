#include "Timer.h"

#include <GLFW/glfw3.h>

namespace luna {

	Timer::Timer() : fps(0.0f), delta(0.0f), lastFrame(0.0f), lastFps(0.0f), frames(0) {}

	float Timer::getTime() const {
		return glfwGetTime();
	}

	float Timer::getDelta() const {
		return delta;
	}

	void Timer::tick() {
		float time = getTime();
		if (lastFps - time > 1.0f) {
			fps = frames;
			frames = 0;
			lastFps = time;
		}
		delta = time - lastFrame;
		lastFrame = time;
		++frames;
	}

}
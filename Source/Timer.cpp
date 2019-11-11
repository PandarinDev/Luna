#include "Timer.h"

#include <GLFW/glfw3.h>

#include <iostream>

namespace luna {

	Timer::Timer() :
		fps(0.0f), delta(0.0f), frames(0), avgFrameTime(0.0f) {
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

	float Timer::getAverageFrameTime() const {
		return avgFrameTime;
	}

	void Timer::tick() {
		float time = getTime();
		if (time - lastFps > 1.0f) {
			fps = frames;
			frames = 0;
			lastFps = time;
			std::cout << "Average frametime is is: " << avgFrameTime * 1000.0f << "ms" << std::endl;
		}
		delta = time - lastFrame;
		avgFrameTime = (avgFrameTime != 0.0f)
			? (avgFrameTime + delta) / 2.0f
			: delta;
		lastFrame = time;
		++frames;
	}

}

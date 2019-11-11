#pragma once

namespace luna {

	class Timer {

	public:

		Timer();

		float getTime() const;
		float getDelta() const;
		int getFps() const;
		float getAverageFrameTime() const;

		void tick();

	private:

		int fps;
		float delta;
		float lastFrame;
		float lastFps;
		int frames;
		float avgFrameTime;

	};

}

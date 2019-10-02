#pragma once

#include "Window.h"

namespace luna {

	class InputManager {

	public:

		InputManager(const Window& window);

	private:

		void keyPressed(int keyCode) const;

	};

}
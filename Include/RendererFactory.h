#pragma once

#include "Renderer.h"

namespace luna {

	class RendererFactory {

	public:

		static Renderer createInstance(float width, float height);

	private:

		RendererFactory() = default;

	};

}
#pragma once

#include "Scene.h"

#include <glm/glm.hpp>

#include <vector>

namespace luna {

	class RenderingStage {

	public:

		virtual ~RenderingStage() = default;

		virtual void render(std::vector<glm::vec3>& pixels, const Scene& scene) = 0;

	};

}
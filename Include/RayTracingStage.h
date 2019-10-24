#pragma once

#include "RenderingStage.h"

#include <random>

namespace luna {

	class RayTracingStage : public RenderingStage {

	public:

		RayTracingStage(float width, float height);

		void render(std::vector<glm::vec3>& pixels, const Scene& scene) const override;

	private:

		float width;
		float height;

		void traceArea(std::vector<glm::vec3>& pixels, const Scene& scene, const glm::vec2& from, const glm::vec2& to) const;

	};

}

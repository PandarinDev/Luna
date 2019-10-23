#pragma once

#include "RenderingStage.h"

namespace luna {

	class RayTracingStage : public RenderingStage {

	public:

		RayTracingStage(float width, float height);

		void render(std::vector<glm::vec3>& pixels, const Scene& scene) const override;

	private:

		float width;
		float height;

	};

}
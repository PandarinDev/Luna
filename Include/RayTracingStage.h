#pragma once

#include "External/ctpl_stl.h"
#include "RenderingStage.h"

namespace luna {

	class RayTracingStage : public RenderingStage {

	public:

		RayTracingStage(float width, float height);

		void render(std::vector<glm::vec3>& pixels, const Scene& scene) override;

	private:

		ctpl::thread_pool threadPool;
		float width;
		float height;

		void traceArea(std::vector<glm::vec3>& pixels, const Scene& scene, const glm::vec2& from, const glm::vec2& to) const;

		static int queryThreads();

	};

}

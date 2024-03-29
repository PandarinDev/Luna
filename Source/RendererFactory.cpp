#include "RendererFactory.h"

#include "RayTracingStage.h"

#include <vector>
#include <memory>

namespace luna {

	Renderer RendererFactory::createInstance(float width, float height) {
		std::vector<std::unique_ptr<RenderingStage>> stages;
		stages.emplace_back(std::make_unique<RayTracingStage>(width, height));
		return { width, height, std::move(stages) };
	}

}

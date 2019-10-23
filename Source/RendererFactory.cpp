#include "RendererFactory.h"

#include "RayTracingStage.h"
#include "FXAAStage.h"

#include <vector>
#include <memory>

namespace luna {

	Renderer RendererFactory::createInstance(float width, float height) {
		std::vector<std::unique_ptr<RenderingStage>> stages;
		stages.emplace_back(std::make_unique<RayTracingStage>(width, height));
		stages.emplace_back(std::make_unique<FXAAStage>(width, height));
		return { width, height, std::move(stages) };
	}

}
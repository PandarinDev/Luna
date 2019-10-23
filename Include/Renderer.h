#pragma once

#include "Scene.h"
#include "RenderingStage.h"

#include <glm/vec3.hpp>

#include <vector>
#include <memory>

namespace luna {

	class Renderer {

	public:

		Renderer(
			float width,
			float height,
			std::vector<std::unique_ptr<RenderingStage>>&& stages);

		void clear() const;
		void render(const Scene& scene) const;

	private:

		glm::vec3 origin;
		float width;
		float height;
		std::vector<std::unique_ptr<RenderingStage>> stages;

		void init() const;
		void drawPixels(const std::vector<glm::vec3>& pixels) const;

	};

}
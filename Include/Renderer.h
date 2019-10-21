#pragma once

#include "Scene.h"

#include <glm/vec3.hpp>

#include <vector>

namespace luna {

	class Renderer {

	public:

		Renderer(float width, float height);

		void clear() const;
		void render(const Scene& scene) const;

	private:

		glm::vec3 origin;
		float width;
		float height;

		void init() const;
		void drawPixels(const std::vector<glm::vec3>& pixels) const;

	};

}
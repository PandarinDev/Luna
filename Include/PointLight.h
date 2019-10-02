#pragma once

#include "Object.h"

#include <glm/vec3.hpp>

#include <vector>
#include <memory>

namespace luna {

	class PointLight {

	public:

		glm::vec3 position;
		glm::vec3 color;
		float radius;

		PointLight(const glm::vec3& position, const glm::vec3& color, float radius);

		float calculateEffectAt(const glm::vec3& position, const std::vector<std::unique_ptr<Object>>& objects) const;

	};

}
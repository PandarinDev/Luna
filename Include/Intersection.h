#pragma once

#include <glm/vec3.hpp>

namespace luna {

	struct Intersection {

		glm::vec3 coordinates;
		glm::vec3 normal;

		Intersection(const glm::vec3& coordinates, const glm::vec3& normal) :
			coordinates(coordinates), normal(normal) {}

	};

}
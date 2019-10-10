#pragma once

#include <glm/vec3.hpp>

#include <iostream>

namespace luna {

	class Ray {

	public:

		glm::vec3 origin;
		glm::vec3 direction;

		Ray(const glm::vec3& origin, const glm::vec3& direction);

		glm::vec3 pointAt(float t) const;

	};

	std::ostream& operator<<(std::ostream& os, const Ray& ray);

}
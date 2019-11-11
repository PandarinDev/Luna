#include "Ray.h"

namespace luna {

	Ray::Ray(const glm::vec3& origin, const glm::vec3& direction) :
		origin(origin), direction(direction) {}

	bool Ray::operator==(const Ray& other) const noexcept {
		return origin == other.origin && direction == other.direction;
	}

	glm::vec3 Ray::pointAt(float t) const {
		return origin + direction * t;
	}

	std::ostream& operator<<(std::ostream& os, const Ray& ray) {
		os << "([" << ray.origin.x << ", " << ray.origin.y << ", " << ray.origin.z << "],";
		os << " [" << ray.direction.x << ", " << ray.direction.y << ", " << ray.direction.z << "])";

		return os;
	}

}
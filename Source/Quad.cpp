#include "Quad.h"

#include <glm/glm.hpp>

#include <cmath>

namespace luna {

	Quad::Quad(const glm::vec3& position, const glm::vec3& normal, float size) :
		Object(Material::DEFAULT), position(position), normal(normal), size(size) {}

	std::optional<Intersection> Quad::getIntersectionPoint(const Ray& ray) const {
		static constexpr float ERROR_MARGIN = 1e-6f;

		float denom = glm::dot(-normal, ray.direction);
		if (denom < ERROR_MARGIN) {
			return std::nullopt;
		}

		auto toCenter = position - ray.origin;
		float t = glm::dot(toCenter, -normal) / denom;
		if (t <= ERROR_MARGIN) {
			return std::nullopt;
		}

		auto intersection = ray.pointAt(t);
		float distanceX = std::abs(intersection.x - position.x);
		float distanceY = std::abs(intersection.y - position.y);
		float distanceZ = std::abs(intersection.z - position.z);
		if (distanceX > size || distanceY > size || distanceZ > size) {
			return std::nullopt;
		}

		return std::make_optional<Intersection>(intersection, normal);
	}

}

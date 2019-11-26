#include "Sphere.h"

#include <glm/glm.hpp>

#include <cmath>

namespace luna {

	Sphere::Sphere(const glm::vec3& position, float radius, const Material& material) :
		Object(material), position(position), radius(radius) {}

	std::optional<Intersection> Sphere::getIntersectionPoint(const Ray& ray) const {
		auto toCenter = position - ray.origin;
		auto t = glm::dot(toCenter, ray.direction);
		auto middlePoint = ray.pointAt(t);

		// If the middle point is further than the radius the ray missed
		auto distance = glm::length(middlePoint - position);
		if (distance > radius) {
			return {};
		}

		auto x = std::sqrt(radius * radius - distance * distance);
		auto t1 = t + x;
		auto t2 = t - x;
		auto t1Point = ray.pointAt(t1);
		auto t2Point = ray.pointAt(t2);

		if (t1 < 0.0f) {
			return (t2 > 0.0f)
				? std::make_optional<Intersection>(t2Point, glm::normalize(t2Point - position))
				: std::nullopt;
		}

		if (t2 < 0.0f) {
			return (t1 > 0.0f)
				? std::make_optional<Intersection>(t1Point, glm::normalize(t1Point - position))
				: std::nullopt;
		}

		return (glm::length(t1Point - ray.origin) < glm::length(t2Point - ray.origin))
			? std::make_optional<Intersection>(t1Point, glm::normalize(t1Point - position))
			: std::make_optional<Intersection>(t2Point, glm::normalize(t2Point - position));
	}

}

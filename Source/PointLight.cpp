#include "PointLight.h"

#include <glm/glm.hpp>

#include <optional>

namespace luna {

	PointLight::PointLight(const glm::vec3& position, const glm::vec3& color, float radius) :
		position(position), color(color), radius(radius) {}

	float distanceBetween(const glm::vec3& p1, const glm::vec3& p2) {
		return glm::length(p1 - p2);
	}

	float PointLight::calculateEffectAt(const glm::vec3& position, const std::vector<std::unique_ptr<Object>>& objects) const {
		static constexpr float ERROR_TOLERANCE = 1e-5f;

		Ray ray { this->position, glm::normalize(position - this->position) };
		// First trace if the light can hit the position
		std::optional<glm::vec3> closestIntersection;
		for (const auto& objPtr : objects) {
			auto intersection = objPtr->getIntersectionPoint(ray);
			if (!intersection || (closestIntersection && distanceBetween(this->position, *closestIntersection) < distanceBetween(this->position, *intersection))) {
				continue;
			}
			closestIntersection = intersection;
		}

		if (!closestIntersection) {
			return 0.0f;
		}

		auto delta = glm::length(position - *closestIntersection);
		if (delta > ERROR_TOLERANCE) {
			return 0.0f;
		}

		auto distance = glm::length(position - this->position);
		if (distance > radius) {
			return 0.0f;
		}

		return distance / radius;
	}

}
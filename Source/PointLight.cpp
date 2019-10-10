#include "PointLight.h"

#include <glm/glm.hpp>

#include <optional>
#include <utility>
#include <functional>
#include <algorithm>

#include <iostream>

namespace luna {

	PointLight::PointLight(const glm::vec3& position, const glm::vec3& color, float radius) :
		position(position), color(color), radius(radius) {}

	float PointLight::calculateEffectAt(const glm::vec3& position, const std::vector<std::unique_ptr<Object>>& objects) const {
		static constexpr float ERROR_TOLERANCE = 1e-4f;
		static constexpr int MAX_BOUNCES = 5;

		Ray initialRay { this->position, glm::normalize(position - this->position) };
		// First trace if the light can hit the position
		auto cast = [&](const Ray& ray) {
			std::pair<Object*, glm::vec3> closestIntersection = std::make_pair(nullptr, glm::vec3());
			for (const auto& objPtr : objects) {
				auto intersection = objPtr->getIntersectionPoint(ray);
				if (!intersection || (closestIntersection.first && glm::length(this->position - closestIntersection.second) < glm::length(this->position - *intersection))) {
					continue;
				}
				closestIntersection = std::make_pair(objPtr.get(), *intersection);
			}
			return closestIntersection;
		};

		int bounces = 0;
		std::function<float(const Ray&)> bounce = [&](const Ray& ray) {
			// If we hit max bounces and didn't hit the target it is not lit
			if (++bounces > MAX_BOUNCES) {
				return 0.0f;
			}

			auto intersection = cast(initialRay);
			// If the ray didn't intersect anything the target is not lit
			if (!intersection.first) {
				return 0.0f;
			}

			auto delta = glm::length(position - intersection.second);
			// If the ray hit something else create a new ray and bounce again
			if (delta > ERROR_TOLERANCE) {
				auto surfaceNormal = intersection.first->getSurfaceNormalAt(intersection.second);
				auto reflectedDirection = ray.direction - 2.0f * glm::dot(ray.direction, surfaceNormal) * surfaceNormal;
				Ray reflectedRay { intersection.second, reflectedDirection };
				return bounce(reflectedRay);
			}

			// Otherwise we've got a hit
			auto distance = glm::length(intersection.second - this->position);
			if (distance > radius) {
				return 0.0f;
			}
			auto effectiveness = (1.0f - (distance / radius)) - ((bounces - 1) * 0.1f);
			return std::clamp(effectiveness, 0.0f, 1.0f);
		};
		return bounce(initialRay);
	}

}
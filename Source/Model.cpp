#include "Model.h"

#include <glm/glm.hpp>

#include <limits>

namespace luna {

	Model::Model(const std::vector<Triangle>& triangles) :
		Object(Material::DEFAULT), triangles(triangles) {}

	std::optional<glm::vec3> Model::getIntersectionPoint(const Ray& ray) const {
		std::vector<glm::vec3> intersections;
		for (const auto& triangle : triangles) {
			auto intersection = triangle.getIntersectionPoint(ray);
			if (intersection) {
				intersections.emplace_back(*intersection);
			}
		}

		// If we didn't have any intersections exit early
		if (intersections.empty()) {
			return std::nullopt;
		}

		// Otherwise return the closest intersection
		float minDistance = std::numeric_limits<float>::max();
		glm::vec3& closestIntersection = intersections[0];
		for (const auto& intersection : intersections) {
			auto distance = glm::length(intersection - ray.origin);
			if (distance < minDistance) {
				minDistance = distance;
				closestIntersection = intersection;
			}
		}

		return closestIntersection;
	}

	glm::vec3 Model::getSurfaceNormalAt(const glm::vec3& point) const {
		return glm::vec3(1.0f, 0.0f, 0.0f); // TODO Implement properly
	}

}
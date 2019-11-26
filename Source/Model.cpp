#include "Model.h"
#include "HashUtils.h"

#include <glm/glm.hpp>

#include <limits>
#include <stdexcept>

namespace luna {

	Model::Model(const std::vector<Triangle>& triangles) :
		Object(Material::DEFAULT), triangles(triangles) {}

	std::optional<Intersection> Model::getIntersectionPoint(const Ray& ray) const {
		std::vector<Intersection> intersections;
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
		Intersection& closestIntersection = intersections[0];
		for (const auto& intersection : intersections) {
			auto distance = glm::length(intersection.coordinates - ray.origin);
			if (distance < minDistance) {
				minDistance = distance;
				closestIntersection = intersection;
			}
		}
		return closestIntersection;
	}

}
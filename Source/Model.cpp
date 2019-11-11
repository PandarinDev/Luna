#include "Model.h"
#include "HashUtils.h"

#include <glm/glm.hpp>

#include <limits>
#include <stdexcept>

namespace luna {

	std::unordered_map<glm::vec3, glm::vec3> Model::TRIANGLE_LOOKUP;
	std::mutex Model::TRIANGLE_LOOKUP_MUTEX;

	Model::Model(const std::vector<Triangle>& triangles) :
		Object(Material::DEFAULT), triangles(triangles) {}

	std::optional<glm::vec3> Model::getIntersectionPoint(const Ray& ray) const {
		std::vector<std::pair<glm::vec3, glm::vec3>> intersections;
		for (const auto& triangle : triangles) {
			auto intersection = triangle.getIntersectionPoint(ray);
			if (intersection) {
				intersections.emplace_back(*intersection, triangle.normal);
			}
		}

		// If we didn't have any intersections exit early
		if (intersections.empty()) {
			return std::nullopt;
		}

		// Otherwise return the closest intersection
		float minDistance = std::numeric_limits<float>::max();
		std::pair<glm::vec3, glm::vec3>& closestIntersection = intersections[0];
		for (const auto& intersection : intersections) {
			auto distance = glm::length(intersection.first - ray.origin);
			if (distance < minDistance) {
				minDistance = distance;
				closestIntersection = intersection;
			}
		}
		{
			std::lock_guard<std::mutex> guard(TRIANGLE_LOOKUP_MUTEX);
			TRIANGLE_LOOKUP.emplace(closestIntersection.first, closestIntersection.second);
		}
		return closestIntersection.first;
	}

	glm::vec3 Model::getSurfaceNormalAt(const glm::vec3& point) const {
		{
			std::lock_guard<std::mutex> guard(TRIANGLE_LOOKUP_MUTEX);
			auto it = TRIANGLE_LOOKUP.find(point);
			if (it != TRIANGLE_LOOKUP.end()) {
				return it->second;
			}
		}
		throw std::runtime_error("Cache miss");
	}

}
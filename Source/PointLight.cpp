#include "PointLight.h"

#include <glm/glm.hpp>

#include <optional>
#include <utility>
#include <functional>
#include <algorithm>

namespace luna {

	PointLight::PointLight(
		const glm::vec3& position,
		const glm::vec3& color,
		float diffuseIntensity,
		float specularIntensity) :
		position(position),
		color(color),
		diffuseIntensity(diffuseIntensity),
		specularIntensity(specularIntensity) {}

	float PointLight::calculateEffectAt(
		const glm::vec3& position,
		const Object& object,
		const std::vector<std::unique_ptr<Object>>& objects) const {
		constexpr float ERROR_TOLERANCE = 1e-5f;

		Ray ray { this->position, glm::normalize(position - this->position) };
		// First trace if the light can hit the position
		std::optional<glm::vec3> closestIntersection;
		for (const auto& objPtr : objects) {
			auto intersection = objPtr->getIntersectionPoint(ray);
			if (!intersection || closestIntersection && glm::length(this->position - *closestIntersection) < glm::length(this->position - *intersection)) {
				continue;
			}
			closestIntersection = *intersection;
		}
		
		// If the object is in shadow the light contribution is zero
		if (!closestIntersection || glm::length(position - *closestIntersection) > ERROR_TOLERANCE) {
			return 0.0f;
		}

		const auto& material = object.getMaterial();
		auto normal = object.getSurfaceNormalAt(position);
		auto toLight = glm::normalize(this->position - position);
		auto lightSurfaceDot = glm::dot(toLight, normal);
		auto diffuseComponent = (lightSurfaceDot > 0.0f)
			? material.diffuseReflectivity * lightSurfaceDot * diffuseIntensity
			: 0.0f;
		
		auto reflected = 2.0f * glm::dot(toLight, normal) * normal - toLight;
		auto reflectedToViewer = glm::dot(reflected, glm::normalize(-position));
		auto specularComponent = (reflectedToViewer > 0.0f)
			? material.specularReflectivity * std::pow(reflectedToViewer, material.shininess) * specularIntensity
			: 0.0f;

		return diffuseComponent + specularComponent;
	}

}
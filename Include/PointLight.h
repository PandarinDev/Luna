#pragma once

#include "Object.h"

#include <glm/vec3.hpp>

#include <vector>
#include <memory>

namespace luna {

	class PointLight {

	public:

		glm::vec3 position;
		glm::vec3 color;
		float diffuseIntensity;
		float specularIntensity;

		PointLight(
			const glm::vec3& position,
			const glm::vec3& color,
			float diffuseIntensity,
			float specularIntensity);

		float calculateEffectAt(
			const glm::vec3& position,
			const Object& object,
			const std::vector<std::unique_ptr<Object>>& objects) const;

	};

}
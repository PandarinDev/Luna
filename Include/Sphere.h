#pragma once

#include "Object.h"
#include "Material.h"

#include <glm/vec3.hpp>

namespace luna {

	class Sphere : public Object {

	public:

		glm::vec3 position;
		float radius;

		Sphere(const glm::vec3& position, float radius, const Material& material = Material::DEFAULT);

		std::optional<glm::vec3> getIntersectionPoint(const Ray& ray) const override;

	};

}
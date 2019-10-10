#pragma once

#include "Object.h"

#include <glm/vec3.hpp>

namespace luna {

	class Quad : public Object {

	public:

		glm::vec3 position;
		glm::vec3 normal;
		float size;

		Quad(const glm::vec3& position, const glm::vec3& normal, float size);

		std::optional<glm::vec3> getIntersectionPoint(const Ray& ray) const override;
		glm::vec3 getSurfaceNormalAt(const glm::vec3& point) const override;

	};

}
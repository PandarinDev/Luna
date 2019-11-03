#pragma once

#include "Object.h"
#include "Triangle.h"

#include <vector>

namespace luna {

	class Model : public Object {

	public:

		Model(const std::vector<Triangle>& triangles);

		std::optional<glm::vec3> getIntersectionPoint(const Ray& ray) const override;
		glm::vec3 getSurfaceNormalAt(const glm::vec3& point) const override;

	private:

		std::vector<Triangle> triangles;

	};

}
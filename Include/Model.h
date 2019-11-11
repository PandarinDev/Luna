#pragma once

#include "Object.h"
#include "Triangle.h"

#include <vector>
#include <mutex>
#include <unordered_map>

namespace luna {

	class Model : public Object {

	public:

		Model(const std::vector<Triangle>& triangles);

		std::optional<glm::vec3> getIntersectionPoint(const Ray& ray) const override;
		glm::vec3 getSurfaceNormalAt(const glm::vec3& point) const override;

	private:

		static std::unordered_map<glm::vec3, glm::vec3> TRIANGLE_LOOKUP;
		static std::mutex TRIANGLE_LOOKUP_MUTEX;

		std::vector<Triangle> triangles;

	};

}
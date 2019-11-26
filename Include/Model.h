#pragma once

#include "Object.h"
#include "Triangle.h"

#include <vector>

namespace luna {

	class Model : public Object {

	public:

		Model(const std::vector<Triangle>& triangles);

		std::optional<Intersection> getIntersectionPoint(const Ray& ray) const override;

	private:

		std::vector<Triangle> triangles;

	};

}
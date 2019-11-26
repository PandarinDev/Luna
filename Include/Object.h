#pragma once

#include "Ray.h"
#include "Material.h"
#include "Intersection.h"

#include <optional>

namespace luna {

	class Object {

	public:

		Object(const Material& material) : material(material) {}
		virtual ~Object() = default;

		virtual std::optional<Intersection> getIntersectionPoint(const Ray& ray) const = 0;

		const Material& getMaterial() const {
			return material;
		}

	private:

		Material material;

	};

}
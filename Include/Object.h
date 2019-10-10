#pragma once

#include "Ray.h"
#include "Material.h"

#include <glm/vec3.hpp>

#include <optional>

namespace luna {

	class Object {

	public:

		Object(const Material& material) : material(material) {}
		virtual ~Object() = default;

		virtual std::optional<glm::vec3> getIntersectionPoint(const Ray& ray) const = 0;
		virtual glm::vec3 getSurfaceNormalAt(const glm::vec3& point) const = 0;

		const Material& getMaterial() const {
			return material;
		}

	private:

		Material material;

	};

}
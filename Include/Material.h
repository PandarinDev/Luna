#pragma once

#include <glm/vec3.hpp>

namespace luna {

	class Material {

	public:

		static Material DEFAULT;

		glm::vec3 diffuseColor;

		Material(const glm::vec3& diffuseColor);

	};

}
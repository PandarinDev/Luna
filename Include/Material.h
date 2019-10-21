#pragma once

#include <glm/vec3.hpp>

namespace luna {

	class Material {

	public:

		static Material DEFAULT;

		glm::vec3 color;
		float specularReflectivity;
		float diffuseReflectivity;
		float ambientReflectivity;
		float shininess;

		Material(
			const glm::vec3& color,
			float specularReflectivity,
			float diffuseReflectivity,
			float ambientReflectivity,
			float shininess);

	};

}
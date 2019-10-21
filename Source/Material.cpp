#include "Material.h"

namespace luna {

	Material::Material(
		const glm::vec3& color,
		float specularReflectivity,
		float diffuseReflectivity,
		float ambientReflectivity,
		float shininess) :
		color(color),
		specularReflectivity(specularReflectivity),
		diffuseReflectivity(diffuseReflectivity),
		ambientReflectivity(ambientReflectivity),
		shininess(shininess) {}

	Material Material::DEFAULT {
		{ 1.0f, 1.0f, 1.0f },
		0.5f, // Specular
		1.0f, // Diffuse
		0.1f, // Ambient
		0.7f, // Shininess
	};

}
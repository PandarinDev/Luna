#include "Material.h"

namespace luna {

	Material::Material(const glm::vec3& diffuseColor) :
		diffuseColor(diffuseColor) {}

	Material Material::DEFAULT{ { 1.0f, 1.0f, 1.0f } };

}
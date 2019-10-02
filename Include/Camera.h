#pragma once

#include <glm/vec3.hpp>

namespace luna {

	class Camera {

	public:

		glm::vec3 position;
		glm::vec3 direction;

		Camera(const glm::vec3& position, const glm::vec3& direction) :
			position(position), direction(direction) {}

	};

}
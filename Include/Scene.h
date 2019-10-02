#pragma once

#include "Camera.h"
#include "Object.h"
#include "PointLight.h"

#include <vector>
#include <memory>

namespace luna {

	class Scene {

	public:

		Camera camera;
		std::vector<std::unique_ptr<Object>> objects;
		std::vector<PointLight> lights;

		Scene(
			const Camera& camera,
			std::vector<std::unique_ptr<Object>>&& objects,
			const std::vector<PointLight>& lights) :
			camera(camera), objects(std::move(objects)), lights(lights) {}

	};

}
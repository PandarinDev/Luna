#include "RayTracingStage.h"

namespace luna {

	constexpr float AMBIENT_LIGHT = 0.1f;

	RayTracingStage::RayTracingStage(float width, float height) :
		width(width), height(height) {}

	void RayTracingStage::render(std::vector<glm::vec3>& pixels, const Scene& scene) const {
		static constexpr float FRUSTRUM_DISTANCE = 1.0f;

		float aspect = width / height;
		for (float y = 0; y < height; ++y) {
			for (float x = 0; x < width; ++x) {
				float normalizedX = ((x / width) - 0.5f) * aspect;
				float normalizedY = (y / height) - 0.5f;
				glm::vec3 direction = glm::normalize(glm::vec3(normalizedX, normalizedY, FRUSTRUM_DISTANCE));
				Ray ray{ scene.camera.position , direction };
				std::optional<glm::vec3> closestIntersection;
				glm::vec3 pixelColor{ 0.0f, 0.0f, 0.0f };
				for (const auto& objPtr : scene.objects) {
					auto intersection = objPtr->getIntersectionPoint(ray);
					// Skip if the ray does not intersect the object or there
					// is already a closer intersection with a different object
					if (!intersection || (closestIntersection && glm::length(*closestIntersection - ray.origin) < glm::length(*intersection - ray.origin))) {
						continue;
					}
					closestIntersection = intersection;
					pixelColor = { 0.0f, 0.0f, 0.0f };

					// Otherwise color the pixel to the material of the object
					for (const auto& light : scene.lights) {
						auto intensity = light.calculateEffectAt(*intersection, *objPtr, scene.objects);
						pixelColor += objPtr->getMaterial().color * light.color * intensity;
					}
					pixelColor += objPtr->getMaterial().color * AMBIENT_LIGHT;
				}
				pixels[y * width + x] = pixelColor;
			}
		}
	}

}
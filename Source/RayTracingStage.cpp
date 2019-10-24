#include "RayTracingStage.h"

#include <type_traits>
#include <thread>
#include <future>

namespace luna {

	constexpr float AMBIENT_LIGHT = 0.1f;

	RayTracingStage::RayTracingStage(float width, float height) :
		width(width), height(height) {}

	void RayTracingStage::render(std::vector<glm::vec3>& pixels, const Scene& scene) const {
		auto hardwareThreads = std::thread::hardware_concurrency();
		if (hardwareThreads % 2 == 1) hardwareThreads -= 1;
		std::vector<std::future<void>> futures(hardwareThreads);
		// Create slices for each thread to process
		auto sliceHeight = height / hardwareThreads;
		for (int i = 0; i < hardwareThreads; ++i) {
			glm::vec2 from(0.0f, i * sliceHeight);
			glm::vec2 to(width, from.y + sliceHeight);
			futures[i] = std::async(std::launch::async, [&, from, to] { traceArea(pixels, scene, from, to);  });
		}
		
		// Join futures
		for (const auto& future : futures) {
			future.wait();
		}		
	}

	void RayTracingStage::traceArea(std::vector<glm::vec3>& pixels, const Scene& scene, const glm::vec2& from, const glm::vec2& to) const {
		constexpr float FRUSTRUM_DISTANCE = 1.0f;
		constexpr float SAMPLES_PER_PIXEL = 4;
		static std::uniform_real_distribution<float> REAL_DISTRIBUTION(0.0f, 1.0f);

		std::random_device randomDevice;
		std::mt19937 randomEngine(randomDevice());
		float aspect = width / height;
		for (float y = from.y; y < to.y; ++y) {
			for (float x = from.x; x < to.x; ++x) {
				glm::vec3 pixelColor { 0.0f, 0.0f, 0.0f };
				for (int i = 0; i < SAMPLES_PER_PIXEL; ++i) {
					float normalizedX = (((x + REAL_DISTRIBUTION(randomEngine)) / width) - 0.5f) * aspect;
					float normalizedY = ((y + REAL_DISTRIBUTION(randomEngine)) / height) - 0.5f;
					glm::vec3 direction = glm::normalize(glm::vec3(normalizedX, normalizedY, FRUSTRUM_DISTANCE));
					Ray ray { scene.camera.position, direction };
					std::optional<glm::vec3> closestIntersection;
					glm::vec3 tempColor { 0.0f, 0.0f, 0.0f };
					for (const auto& objPtr : scene.objects) {
						auto intersection = objPtr->getIntersectionPoint(ray);
						// Skip if the ray does not intersect the object or there
						// is already a closer intersection with a different object
						if (!intersection || (closestIntersection && glm::length(*closestIntersection - ray.origin) < glm::length(*intersection - ray.origin))) {
							continue;
						}
						closestIntersection = intersection;
						tempColor = { 0.0f, 0.0f, 0.0f };

						// Otherwise color the pixel to the material of the object
						for (const auto& light : scene.lights) {
							auto intensity = light.calculateEffectAt(*intersection, *objPtr, scene.objects);
							tempColor += objPtr->getMaterial().color * light.color * intensity;
						}
						tempColor += objPtr->getMaterial().color * AMBIENT_LIGHT;
					}
					pixelColor += tempColor;
				}
				pixels[y * width + x] = pixelColor / SAMPLES_PER_PIXEL;
			}
		}
	}

}

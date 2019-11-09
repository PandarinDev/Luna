#include "RayTracingStage.h"
#include "SamplingStrategy.h"

#include <type_traits>
#include <thread>
#include <future>

namespace luna {

	constexpr float AMBIENT_LIGHT = 0.1f;

	RayTracingStage::RayTracingStage(float width, float height) :
		threadPool(queryThreads()), width(width), height(height) {}

	void RayTracingStage::render(std::vector<glm::vec3>& pixels, const Scene& scene) {
		auto numThreads = threadPool.size();
		std::vector<std::future<void>> futures(numThreads);
		// Create slices for each thread to process
		auto sliceHeight = height / numThreads;
		for (int i = 0; i < numThreads; ++i) {
			glm::vec2 from(0.0f, i * sliceHeight);
			glm::vec2 to(width, from.y + sliceHeight);
			futures[i] = threadPool.push([&, from, to](int) { traceArea(pixels, scene, from, to); });
		}
		
		// Join futures
		for (const auto& future : futures) {
			future.wait();
		}
	}

	void RayTracingStage::traceArea(std::vector<glm::vec3>& pixels, const Scene& scene, const glm::vec2& from, const glm::vec2& to) const {
		constexpr float FRUSTRUM_DISTANCE = 1.0f;
		constexpr auto SAMPLING_STRATEGY = SamplingStrategy::AA_0_SAMPLING;
		constexpr auto SAMPLES_PER_PIXEL = Sampler::getTotalNumberOfIterations<SAMPLING_STRATEGY>();

		float aspect = width / height;
		for (float y = from.y; y < to.y; ++y) {
			for (float x = from.x; x < to.x; ++x) {
				glm::vec3 pixelColor { 0.0f, 0.0f, 0.0f };
				for (int i = 0; i < SAMPLES_PER_PIXEL; ++i) {
					float offset = Sampler::getOffset<SAMPLING_STRATEGY>(i);
					float normalizedX = (((x + offset) / width) - 0.5f) * aspect;
					float normalizedY = ((y + offset) / height) - 0.5f;
					glm::vec3 direction = glm::normalize(glm::vec3(normalizedX, normalizedY, FRUSTRUM_DISTANCE));
					Ray ray { scene.camera.position, direction };
					std::optional<glm::vec3> closestIntersection;
					Object* closestObject = nullptr;
					for (const auto& objPtr : scene.objects) {
						auto intersection = objPtr->getIntersectionPoint(ray);
						// Skip if the ray does not intersect the object or there
						// is already a closer intersection with a different object
						if (!intersection || (closestIntersection && glm::length(*closestIntersection - ray.origin) < glm::length(*intersection - ray.origin))) {
							continue;
						}
						closestIntersection = intersection;
						closestObject = objPtr.get();
					}
					// Avoid lighting calculations if we didn't hit anything
					if (!closestIntersection) {
						continue;
					}
					// Otherwise color the pixel to the material of the object
					for (const auto& light : scene.lights) {
						auto intensity = light.calculateEffectAt(*closestIntersection, *closestObject, scene.objects);
						pixelColor += closestObject->getMaterial().color * light.color * intensity;
					}
					pixelColor += closestObject->getMaterial().color * AMBIENT_LIGHT;
				}
				pixels[y * width + x] = pixelColor / (float) SAMPLES_PER_PIXEL;
			}
		}
	}

	int RayTracingStage::queryThreads() {
		auto hardwareThreads = std::thread::hardware_concurrency();

		// We always want to work with an even number of threads
		if (hardwareThreads % 2 == 1) {
			hardwareThreads -= 1;
		}

		return hardwareThreads;
	}

}

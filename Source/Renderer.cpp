#include "Renderer.h"

#include "External/glad.h"
#include "Sphere.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace luna {
	
	constexpr float Renderer::AMBIENT_LIGHT;

	Renderer::Renderer(float width, float height) :
		origin(0.0f, 0.0f, 0.0f), width(width), height(height) {
		init();
	}

	void Renderer::clear() const {
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::render(const Scene& scene) const {
		static constexpr float FRUSTRUM_DISTANCE = 1.0f;

		std::vector<glm::vec3> pixels;
		pixels.reserve(static_cast<std::size_t>(width * height));
		float aspect = width / height;
		for (float y = 0; y < height; ++y) {
			for (float x = 0; x < width; ++x) {
				float normalizedX = ((x / width) - 0.5f) * aspect;
				float normalizedY = (y / height) - 0.5f;
				glm::vec3 direction = glm::normalize(glm::vec3(normalizedX, normalizedY, FRUSTRUM_DISTANCE));
				Ray ray { scene.camera.position , direction };
				std::optional<glm::vec3> closestIntersection;
				glm::vec3 pixelColor { 0.0f, 0.0f, 0.0f };
				for (const auto& objPtr : scene.objects) {
					auto intersection = objPtr->getIntersectionPoint(ray);
					// Skip if the ray does not intersect the object or there
					// is already a closer intersection with a different object
					if (!intersection || (closestIntersection && glm::length(*closestIntersection - ray.origin) < glm::length(*intersection - ray.origin))) {
						continue;
					}
					closestIntersection = intersection;

					// Otherwise color the pixel to the material of the object
					for (const auto& light : scene.lights) {
						auto intensity = light.calculateEffectAt(*intersection, scene.objects);
						pixelColor += light.color * intensity;
					}
					pixelColor += objPtr->getMaterial().diffuseColor * AMBIENT_LIGHT;
				}

				for (const auto& light : scene.lights) {
					Sphere lightSphere(light.position, light.radius, {{ 0.0f, 0.0f, 1.0f }});
					auto intersection = lightSphere.getIntersectionPoint(ray);
					if (!intersection || (closestIntersection && glm::length(*closestIntersection - ray.origin) < glm::length(*intersection - ray.origin))) {
						continue;
					}
					closestIntersection = intersection;

					pixelColor += lightSphere.getMaterial().diffuseColor * 0.3f;
				}
				pixels.emplace_back(pixelColor);
			}
		}

		drawPixels(pixels);
	}

	void Renderer::init() const {
		gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
		glMatrixMode(GL_PROJECTION_MATRIX);
		glLoadIdentity();
		glm::mat4 orthoMatrix = glm::ortho(0.0f, width, 0.0f, height);
		glLoadMatrixf(&orthoMatrix[0][0]);
	}

	void Renderer::drawPixels(const std::vector<glm::vec3>& pixels) const {
		glBegin(GL_POINTS);
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				const auto& pixel = pixels[y * width + x];
				glColor3f(pixel.r, pixel.g, pixel.b);
				glVertex2i(x, y);
			}
		}
		glEnd();
	}

}
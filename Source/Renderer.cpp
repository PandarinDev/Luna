#include "Renderer.h"

#include "External/glad.h"
#include "Sphere.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace luna {

	Renderer::Renderer(
		float width,
		float height,
		std::vector<std::unique_ptr<RenderingStage>>&& stages) :
		origin(0.0f, 0.0f, 0.0f), width(width), height(height), stages(std::move(stages)) {
		init();
	}

	void Renderer::clear() const {
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::render(const Scene& scene) const {
		std::vector<glm::vec3> pixels(width * height);
		for (const auto& stage : stages) {
			stage->render(pixels, scene);
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
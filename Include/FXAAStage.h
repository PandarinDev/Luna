#pragma once

#include "RenderingStage.h"

#include <glm/vec3.hpp>

#include <tuple>

namespace luna {

	class FXAAStage : public RenderingStage {

	public:

		static const glm::vec3 LUMA_CONSTANTS;

		FXAAStage(float width, float height);

		void render(std::vector<glm::vec3>& pixels, const Scene& scene) const override;

	private:

		float width;
		float height;

		glm::vec3& getPixel(std::vector<glm::vec3>& pixels, float x, float y) const;
		glm::vec3 getPixelColor(std::vector<glm::vec3>& pixels, float x, float y) const;
		float rgbToLuma(const glm::vec3& rgb) const;
		float offsetMultiplierForIteration(int i) const;

		// Returns the pixel's direct neighbors in order: Top, right, bottom, left
		std::tuple<
			glm::vec3&, glm::vec3&,
			glm::vec3&, glm::vec3&> directNeighbors(std::vector<glm::vec3>& pixels, float x, float y) const;

		// Returns the pixel's diagonal neighbors in the order: Top right, bottom right, bottom left, top left
		std::tuple<
			glm::vec3&, glm::vec3&,
			glm::vec3&, glm::vec3&> diagonalNeighbors(std::vector<glm::vec3>& pixels, float x, float y) const;

	};

}
#include "FXAAStage.h"

#include <cmath>
#include <algorithm>

namespace luna {

	// These are all constant values recommended in Timothy Lottes' paper on FXAA
	const glm::vec3 FXAAStage::LUMA_CONSTANTS{ 0.299f, 0.587f, 0.114f };
	constexpr float EDGE_THRESHOLD_MIN = 0.0312f;
	constexpr float EDGE_THRESHOLD_MAX = 0.125f;
	constexpr int MAX_EDGE_ITERATIONS = 12;

	FXAAStage::FXAAStage(float width, float height) :
		width(width), height(height) {}

	void FXAAStage::render(std::vector<glm::vec3>& pixels, const Scene& scene) const {
		for (float y = 1; y < height - 1; ++y) {
			for (float x = 1; x < width - 1; ++x) {
				glm::vec2 coords(x, y);
				auto& pixel = getPixel(pixels, x, y);
				auto [top, right, bottom, left] = directNeighbors(pixels, x, y);
				float lumaCenter = rgbToLuma(pixel);
				float lumaTop = rgbToLuma(top);
				float lumaRight = rgbToLuma(right);
				float lumaBottom = rgbToLuma(bottom);
				float lumaLeft = rgbToLuma(left);

				// Check if we need to apply anti-aliasing for this pixel
				float lumaMin = std::min(lumaCenter, std::min(lumaTop, std::min(lumaRight, std::min(lumaBottom, lumaLeft))));
				float lumaMax = std::max(lumaCenter, std::max(lumaTop, std::max(lumaRight, std::max(lumaBottom, lumaLeft))));
				float lumaRange = lumaMax - lumaMin;
				
				// If we are below the edge threshold or in a really dark area do not modify the pixel color
				if (lumaRange < std::max(EDGE_THRESHOLD_MIN, lumaMax * EDGE_THRESHOLD_MAX)) {
					continue;
				}

				// Check if this is a horizontal or a vertical edge
				auto [topRight, bottomRight, bottomLeft, topLeft] = diagonalNeighbors(pixels, x, y);
				float lumaTopRight = rgbToLuma(topRight);
				float lumaBottomRight = rgbToLuma(bottomRight);
				float lumaBottomLeft = rgbToLuma(bottomLeft);
				float lumaTopLeft = rgbToLuma(topLeft);

				float lumaDownUp = lumaBottom + lumaTop;
				float lumaLeftRight = lumaLeft + lumaRight;
				float lumaLeftCorners = lumaBottomLeft + lumaTopLeft;
				float lumaDownCorners = lumaBottomLeft + lumaBottomRight;
				float lumaRightCorners = lumaBottomRight + lumaTopRight;
				float lumaUpCorners = lumaTopRight + lumaTopLeft;
				
				float edgeHorizontal = std::abs(-2.0f * lumaLeft + lumaLeftCorners)
					+ std::abs(-2.0f * lumaCenter + lumaDownUp) * 2.0f
					+ std::abs(-2.0f * lumaRight + lumaRightCorners);
				float edgeVertical = std::abs(-2.0f * lumaTop + lumaUpCorners)
					+ std::abs(-2.0f * lumaCenter + lumaLeftRight) * 2.0f
					+ std::abs(-2.0f * lumaBottom + lumaDownCorners);
				bool isHorizontal = (edgeHorizontal >= edgeVertical);

				// Choosing edge orientation
				float luma1 = isHorizontal ? lumaBottom : lumaLeft;
				float luma2 = isHorizontal ? lumaTop : lumaRight;
				float gradient1 = luma1 - lumaCenter;
				float gradient2 = luma2 - lumaCenter;
				bool is1Steepest = std::abs(gradient1) >= std::abs(gradient2);
				float gradientScaled = 0.25f * std::max(std::abs(gradient1), std::abs(gradient2));
				float lumaLocalAverage = is1Steepest
					? 0.5f * (luma1 + lumaCenter)
					: 0.5f * (luma2 + lumaCenter);

				// Shift our coordinates to the be at the right side of the current pixel
				glm::vec2 offset(0.0f, 0.0f);
				if (isHorizontal) {
					coords.y += is1Steepest ? -0.5f : 0.5f;
					offset.x = is1Steepest ? -1.0f : 1.0f;
				}
				else {
					coords.x += is1Steepest ? -0.5f : 0.5f;
					offset.y = is1Steepest ? -1.0f : 1.0f;
				}
				coords += offset * 0.5f;

				// Iterate along the edge, trying to find it's end by comparing local luma to the local average
				// Note: 1 in this case corresponds to either down or left, 2 corresponds to either up or right
				// depending on whether the edge is horizontal or vertical.
				bool reached1 = false;
				bool reached2 = false;
				float lumaEnd1 = 0.0f;
				float lumaEnd2 = 0.0f;
				glm::vec2 uv1 = coords;
				glm::vec2 uv2 = coords;
				for (int i = 1; i < MAX_EDGE_ITERATIONS; ++i) {
					if (!reached1) {
						uv1 -= offset * offsetMultiplierForIteration(i);
						// Do an out-of-bounds check
						if (uv1.x < 0.0f || uv1.y < 0.0f || uv1.x > width - 1.0f || uv1.y > height - 1.0f) {
							reached1 = true;
						}
						else {
							lumaEnd1 = rgbToLuma(getPixel(pixels, uv1.x, uv1.y)) - lumaLocalAverage;
						}
					}

					if (!reached2) {
						uv2 += offset * offsetMultiplierForIteration(i);
						if (uv2.x < 0.0f || uv2.y < 0.0f || uv2.x > width - 1.0f || uv2.y > height - 1.0f) {
							reached2 = true;
						}
						else {
							lumaEnd2 = rgbToLuma(getPixel(pixels, uv2.x, uv2.y)) - lumaLocalAverage;
						}
					}

					reached1 = std::abs(lumaEnd1) >= gradientScaled;
					reached2 = std::abs(lumaEnd2) >= gradientScaled;

					// If we found both endpoints exit the loop
					if (reached1 && reached2) {
						break;
					}
				}

				// Estimate the offset from the extremeties of the edge
				float distance1 = isHorizontal ? coords.x - uv1.x : coords.y - uv1.y;
				float distance2 = isHorizontal ? uv2.x - coords.x : uv2.y - coords.y;
				bool isDirection1 = (distance1 < distance2);
				float finalDistance = std::min(distance1, distance2);
				float edgeLength = distance1 + distance2;
				// Prevent division by zero
				if (edgeLength == 0.0f) {
					continue;
				}
				float pixelOffset = -finalDistance / edgeLength + 0.5f;

				// Ensure that the luma variation is correct
				bool isLumaCenterSmaller = (lumaCenter < lumaLocalAverage);
				bool correctVariation = ((isDirection1 ? lumaEnd1 : lumaEnd2) < 0.0f) != isLumaCenterSmaller;
				float finalOffset = correctVariation ? pixelOffset : 0.0f;
				glm::vec2 finalCoords(x + 0.5f, y + 0.5f);
				if (isHorizontal) {
					finalCoords.y += is1Steepest ? -finalOffset : finalOffset; // TODO Check for is1Steeper to fix direction
				}
				else {
					finalCoords.x += is1Steepest ? -finalOffset : finalOffset;
				}

				pixel = getPixelColor(pixels, finalCoords.x, finalCoords.y);
			}
		}
	}

	glm::vec3& FXAAStage::getPixel(std::vector<glm::vec3>& pixels, float x, float y) const {
		return pixels[std::round(y) * width + std::round(x)];
	}

	glm::vec3 FXAAStage::getPixelColor(std::vector<glm::vec3>& pixels, float x, float y) const {
		float xRemainder = std::fmod(x, 1.0f);
		float yRemainder = std::fmod(y, 1.0f);
		auto color = getPixel(pixels, x, y);
		if (xRemainder > 0.5f) {
			auto weight = (xRemainder - 0.5f) * 2.0f;
			auto rightColor = getPixel(pixels, x + 1, y);
			color = color * (1.0f - weight) + rightColor * weight;
		}
		if (xRemainder < 0.5f) {
			auto weight = (0.5f - xRemainder) * 2.0f;
			auto leftColor = getPixel(pixels, x - 1, y);
			color = color * (1.0f - weight) + leftColor * weight;
		}
		if (yRemainder > 0.5f) {
			auto weight = (yRemainder - 0.5f) * 2.0f;
			auto bottomColor = getPixel(pixels, x, y + 1);
			color = color * (1.0f - weight) + bottomColor * weight;
		}
		if (yRemainder < 0.5f) {
			auto weight = (0.5f - yRemainder) * 2.0f;
			auto topColor = getPixel(pixels, x, y - 1);
			color = color * (1.0f - weight) + topColor * weight;
		}

		return color;
	}

	float FXAAStage::rgbToLuma(const glm::vec3& rgb) const {
		return std::sqrt(glm::dot(rgb, LUMA_CONSTANTS));
	}

	float FXAAStage::offsetMultiplierForIteration(int i) const {
		return i;
	}

	std::tuple<
		glm::vec3&, glm::vec3&,
		glm::vec3&, glm::vec3&> FXAAStage::directNeighbors(std::vector<glm::vec3>& pixels, float x, float y) const {
		auto& top = getPixel(pixels, x, y - 1);
		auto& right = getPixel(pixels, x + 1, y);
		auto& bottom = getPixel(pixels, x, y + 1);
		auto& left = getPixel(pixels, x - 1, y);
		return std::forward_as_tuple(top, right, bottom, left);
	}

	std::tuple<
		glm::vec3&, glm::vec3&,
		glm::vec3&, glm::vec3&> FXAAStage::diagonalNeighbors(std::vector<glm::vec3>& pixels, float x, float y) const {
		auto& topRight = getPixel(pixels, x + 1, y - 1);
		auto& bottomRight = getPixel(pixels, x + 1, y + 1);
		auto& bottomLeft = getPixel(pixels, x - 1, y + 1);
		auto& topLeft = getPixel(pixels, x - 1, y - 1);
		return std::forward_as_tuple(topRight, bottomRight, bottomLeft, topLeft);
	}

}
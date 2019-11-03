#include "Triangle.h"

#include <glm/glm.hpp>

#include <iostream>

namespace luna {

    Triangle::Triangle(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2) :
        Object(Material::DEFAULT), p0(p0), p1(p1), p2(p2) {}

    std::optional<glm::vec3> Triangle::getIntersectionPoint(const Ray& ray) const {
        constexpr float ERROR_MARGIN = 1e-5f;

        auto normal = -getSurfaceNormalAt(ray.origin);
        float denom = glm::dot(normal, normal);

        // Check if the ray and the plane are parallel
        float nDotRay = glm::dot(normal, ray.direction);
        if (std::fabs(nDotRay) < ERROR_MARGIN) {
            return std::nullopt;
        }

        float d = glm::dot(normal, p0);
        float t = (glm::dot(-normal, ray.origin) + d) / nDotRay; // TODO Why is -normal required here?

        // Check if the triangle is behind the ray
        if (t < 0.0f) {
            return std::nullopt;
        }

        // Compute ray-plane intersection
        auto intersection = ray.pointAt(t);

        // Do inside-out testing for the triangle
        // Edge 0
        auto edge0 = p1 - p0;
        auto ip0 = intersection - p0;
        auto c0 = glm::cross(edge0, ip0);
        if (glm::dot(normal, c0) < 0.0f) {
            return std::nullopt;
        }

        // Edge 1
        auto edge1 = p2 - p1;
        auto ip1 = intersection - p1;
        auto c1 = glm::cross(edge1, ip1);
        if (glm::dot(normal, c1) < 0.0f) {
            return std::nullopt;
        }

        // Edge 2
        auto edge2 = p0 - p2;
        auto ip2 = intersection - p2;
        auto c2 = glm::cross(edge2, ip2);
        if (glm::dot(normal, c2) < 0.0f) {
            return std::nullopt;
        }

        return intersection;
    }

    glm::vec3 Triangle::getSurfaceNormalAt(const glm::vec3& point) const {
        return glm::cross(p1 - p0, p0 - p2);
    }

}
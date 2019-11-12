#include "Triangle.h"

#include <glm/glm.hpp>

#include <iostream>

namespace luna {

    Triangle::Triangle(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2) :
        Object(Material::DEFAULT), p0(p0), p1(p1), p2(p2),
        normal(glm::normalize(glm::cross(p1 - p0, p2 - p0))), p0p1(p1 - p0), p0p2(p2 - p0) {}

    std::optional<glm::vec3> Triangle::getIntersectionPoint(const Ray& ray) const {
        constexpr float ERROR_MARGIN = 1e-5f;

        glm::vec3 pvec = glm::cross(ray.direction, p0p2);
        float det = glm::dot(p0p1, pvec);
        if (det < ERROR_MARGIN) {
            return std::nullopt;
        }

        float invDet = 1.0f / det;
        glm::vec3 tvec = ray.origin - p0;
        float u = glm::dot(tvec, pvec) * invDet;
        if (u < 0.0f || u > 1.0f) {
            return std::nullopt;
        }

        glm::vec3 qvec = glm::cross(tvec, p0p1);
        float v = glm::dot(ray.direction, qvec) * invDet;
        if (v < 0.0f || u + v > 1.0f) {
            return std::nullopt;
        }

        return ray.pointAt(glm::dot(p0p2, qvec) * invDet);
    }

    glm::vec3 Triangle::getSurfaceNormalAt(const glm::vec3& point) const {
        return normal;
    }

}
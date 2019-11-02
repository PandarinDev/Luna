#pragma once

#include "Object.h"

#include <glm/vec3.hpp>

namespace luna {

    class Triangle : public Object {

    public:

        const glm::vec3 p0;
        const glm::vec3 p1;
        const glm::vec3 p2;

        Triangle(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2);

        std::optional<glm::vec3> getIntersectionPoint(const Ray& ray) const override;
		glm::vec3 getSurfaceNormalAt(const glm::vec3& point) const override;

    };

}
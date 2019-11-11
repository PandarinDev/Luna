#pragma once

#include <functional>

namespace luna {

    namespace HashUtils {

        template<typename T>
        void hashCombine(std::size_t& seed, const T& value) {
            static std::hash<T> hasher;
            seed ^= hasher(value) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }

    }

}

// Hash specializations
namespace std {

	template<>
	struct hash<glm::vec3> {

		std::size_t operator()(const glm::vec3& v) const noexcept {
			using namespace luna::HashUtils;
			std::size_t result = std::hash<float>()(v.x);
			hashCombine(result, v.y);
			hashCombine(result, v.z);
			return result;
		}

	};

}
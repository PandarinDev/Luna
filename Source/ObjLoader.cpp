#include "ObjLoader.h"
#include "StringUtils.h"
#include "Triangle.h"

#include <glm/vec3.hpp>

#include <fstream>
#include <stdexcept>

namespace luna {

	std::vector<std::unique_ptr<Object>> ObjLoader::loadObjFile(const std::string& filePath) {
		std::ifstream fileHandle(filePath);
		if (!fileHandle) {
			throw std::runtime_error("Failed to open file '" + filePath + "'.");
		}

		std::string line;
		std::vector<glm::vec3> vertices;
		std::vector<std::unique_ptr<Object>> triangles;
		while (std::getline(fileHandle, line)) {
			if (StringUtils::startsWith(line, "v ")) {
				auto entries = StringUtils::split(line, ' ');
				float x = std::stof(entries[1]);
				float y = std::stof(entries[2]);
				float z = std::stof(entries[3]);
				vertices.emplace_back(x, y, z);
			}
			else if (StringUtils::startsWith(line, "f ")) {
				auto entries = StringUtils::split(line, ' ');
				// Vertices in OBJs are indexed from 1 so we have to subtract here
				int i0 = std::stoi(entries[1]) - 1;
				int i1 = std::stoi(entries[2]) - 1;
				int i2 = std::stoi(entries[3]) - 1;
				auto& v0 = vertices[i0];
				auto& v1 = vertices[i1];
				auto& v2 = vertices[i2];
				triangles.emplace_back(std::make_unique<Triangle>(v0, v1, v2));
			}
		}

		return triangles;
	}

}
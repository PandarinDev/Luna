#pragma once

#include "Object.h"

#include <vector>
#include <string>
#include <memory>

namespace luna {

	class ObjLoader {

	public:

		static std::vector<std::unique_ptr<Object>> loadObjFile(const std::string& filePath);

	private:

		ObjLoader() = default;

	};

}
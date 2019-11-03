#pragma once

#include <string>
#include <vector>

namespace luna {

	class StringUtils {

	public:

		static std::vector<std::string> split(const std::string& input, char delimiter);
		static bool startsWith(const std::string& input, const std::string& match);

	private:

		StringUtils() = default;

	};

}
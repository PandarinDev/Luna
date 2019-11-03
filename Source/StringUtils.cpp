#include "StringUtils.h"

#include <sstream>

namespace luna {

	std::vector<std::string> StringUtils::split(const std::string& input, char delimiter) {
		std::vector<std::string> results;
		std::string entry;
		std::stringstream inputStream(input);
		while (std::getline(inputStream, entry, delimiter)) {
			results.emplace_back(std::move(entry));
		}

		return results;
	}

	bool StringUtils::startsWith(const std::string& input, const std::string& match) {
		return input.substr(0, match.length()) == match;
	}

}